//
// Created by shuru on 7/15/2019.
//

#include "ReflectionalSpirV.h"
#include "../../Logging/Logging.h"
#include "VulkanBackend.h"

#include <spirv_cross/spirv_glsl.hpp>

ReflectionalSpirV::ReflectionalSpirV(VkDevice device, VkPhysicalDevice physicalDevice)
{
    this->device = device;
    this->physicalDevice = physicalDevice;
    shaderModule = nullptr;
}

ReflectionalSpirV::~ReflectionalSpirV()
{
    if (shaderModule)
    {
        vkDestroyShaderModule(device, shaderModule, nullptr);
    }
}

VkShaderModule ReflectionalSpirV::GetShaderModule()
{
    return shaderModule;
}

bool ReflectionalSpirV::LoadShaderFile(const boost::container::string& shaderFile)
{
    boost::filesystem::ifstream file(boost::filesystem::path(shaderFile.c_str()), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        LOG_FATAL << "Failed to open shader file: " << shaderFile;
        throw std::runtime_error("Failed to open shader file!");
    }

    size_t fileSize = (size_t) file.tellg();
    boost::container::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    // Copy data from the char-vector to a new uint32_t-vector
    std::vector<uint32_t> spirv_binary(buffer.size() / sizeof(uint32_t) * sizeof(char));
    memcpy(spirv_binary.data(), buffer.data(), buffer.size());

    shaderValid = CreateShader(spirv_binary);
    if (!shaderValid)
    {
        LOG_ERROR << "Error loading shader " << shaderFile << ", shader not valid!";
        return false;
    }

    spirv_cross::Compiler compiler(std::move(spirv_binary));

    // The SPIR-V is now parsed, and we can perform reflection on it.
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    constantBufferCount = resources.uniform_buffers.size();
    constantBuffers = new ReflectionalConstantBuffer[constantBufferCount * vulkanBackend->GetSwapChainImageCount()];
    constantBuffersMemory.resize(constantBufferCount * vulkanBackend->GetSwapChainImageCount());

    // Handle bound resources (like textures and samplers)
//    unsigned int resourceCount = shaderDesc.BoundResources;
//    for (unsigned int r = 0; r < resourceCount; r++)
//    {
//        // Get this resource's description
//        D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
//        refl->GetResourceBindingDesc(r, &resourceDesc);
//
//        // Check the type
//        switch (resourceDesc.Type)
//        {
//            case D3D_SIT_TEXTURE: // A texture resource
//            {
//                // Create the SRV wrapper
//                SimpleSRV* srv = new SimpleSRV();
//                srv->BindIndex = resourceDesc.BindPoint;				// Shader bind point
//                srv->Index = (unsigned int)shaderResourceViews.size();	// Raw index
//
//                textureTable.insert(std::pair<std::string, SimpleSRV*>(resourceDesc.Name, srv));
//                shaderResourceViews.push_back(srv);
//            }
//                break;
//
//            case D3D_SIT_SAMPLER: // A sampler resource
//            {
//                // Create the sampler wrapper
//                SimpleSampler* samp = new SimpleSampler();
//                samp->BindIndex = resourceDesc.BindPoint;			// Shader bind point
//                samp->Index = (unsigned int)samplerStates.size();	// Raw index
//
//                samplerTable.insert(std::pair<std::string, SimpleSampler*>(resourceDesc.Name, samp));
//                samplerStates.push_back(samp);
//            }
//                break;
//        }
//    }

    // Loop through all constant buffers
    for (unsigned int b = 0; b < constantBufferCount; b++)
    {
        // Get this buffer
        auto& resource = resources.uniform_buffers[b];
        unsigned int imageCount = vulkanBackend->GetSwapChainImageCount();

        const spirv_cross::SPIRType& type = compiler.get_type(resource.base_type_id);
        VkDeviceSize bufferSize = compiler.get_declared_struct_size(type);
        for (unsigned int s = 0; s < imageCount; ++s)
        {
            // Set up the buffer and put its pointer in the table
            constantBuffers[b * imageCount + s].SetIndex = compiler.get_decoration(resource.id,
                                                                                   spv::DecorationDescriptorSet);
            constantBuffers[b * imageCount + s].BindIndex = compiler.get_decoration(resource.id,
                                                                                    spv::DecorationBinding);
            constantBuffers[b * imageCount + s].LocationIndex = compiler.get_decoration(resource.id,
                                                                                        spv::DecorationLocation);
            constantBuffers[b * imageCount + s].Name = boost::container::string(resource.name.c_str()) + "_" +
                                                       boost::lexical_cast<boost::container::string>(s);
            cbTable.insert(std::pair<boost::container::string, ReflectionalConstantBuffer*>(
                    constantBuffers[b * imageCount + s].Name,
                    &constantBuffers[b * imageCount + s]));

            // Create this constant buffer

        }
    }

    LOG_INFO << "Shader file " << shaderFile << " loaded.";

    return false;
}

void ReflectionalSpirV::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                     VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t ReflectionalSpirV::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    LOG_FATAL << "Failed to find suitable memory type!";
    throw std::runtime_error("Failed to find suitable memory type!");
}
