//
// Created by shuru on 7/15/2019.
//
#include <boost/algorithm/string/predicate.hpp>
#include "ReflectionalSpirV.h"
#include "../../Logging/Logging.h"
#include "VulkanBackend.h"

// Returns the size in bytes of the provided VkFormat.
// As this is only intended for vertex attribute formats, not all VkFormats are supported.
static uint32_t FormatSize(VkFormat format)
{
    uint32_t result = 0;
    switch (format)
    {
        case VK_FORMAT_UNDEFINED:
            result = 0;
            break;
        case VK_FORMAT_R4G4_UNORM_PACK8:
            result = 1;
            break;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            result = 2;
            break;
        case VK_FORMAT_R8_UNORM:
            result = 1;
            break;
        case VK_FORMAT_R8_SNORM:
            result = 1;
            break;
        case VK_FORMAT_R8_USCALED:
            result = 1;
            break;
        case VK_FORMAT_R8_SSCALED:
            result = 1;
            break;
        case VK_FORMAT_R8_UINT:
            result = 1;
            break;
        case VK_FORMAT_R8_SINT:
            result = 1;
            break;
        case VK_FORMAT_R8_SRGB:
            result = 1;
            break;
        case VK_FORMAT_R8G8_UNORM:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SNORM:
            result = 2;
            break;
        case VK_FORMAT_R8G8_USCALED:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SSCALED:
            result = 2;
            break;
        case VK_FORMAT_R8G8_UINT:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SINT:
            result = 2;
            break;
        case VK_FORMAT_R8G8_SRGB:
            result = 2;
            break;
        case VK_FORMAT_R8G8B8_UNORM:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SNORM:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_USCALED:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SSCALED:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_UINT:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SINT:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8_SRGB:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_UNORM:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SNORM:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_USCALED:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SSCALED:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_UINT:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SINT:
            result = 3;
            break;
        case VK_FORMAT_B8G8R8_SRGB:
            result = 3;
            break;
        case VK_FORMAT_R8G8B8A8_UNORM:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SNORM:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_USCALED:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_UINT:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SINT:
            result = 4;
            break;
        case VK_FORMAT_R8G8B8A8_SRGB:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_UNORM:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SNORM:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_USCALED:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_UINT:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SINT:
            result = 4;
            break;
        case VK_FORMAT_B8G8R8A8_SRGB:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_R16_UNORM:
            result = 2;
            break;
        case VK_FORMAT_R16_SNORM:
            result = 2;
            break;
        case VK_FORMAT_R16_USCALED:
            result = 2;
            break;
        case VK_FORMAT_R16_SSCALED:
            result = 2;
            break;
        case VK_FORMAT_R16_UINT:
            result = 2;
            break;
        case VK_FORMAT_R16_SINT:
            result = 2;
            break;
        case VK_FORMAT_R16_SFLOAT:
            result = 2;
            break;
        case VK_FORMAT_R16G16_UNORM:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SNORM:
            result = 4;
            break;
        case VK_FORMAT_R16G16_USCALED:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SSCALED:
            result = 4;
            break;
        case VK_FORMAT_R16G16_UINT:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SINT:
            result = 4;
            break;
        case VK_FORMAT_R16G16_SFLOAT:
            result = 4;
            break;
        case VK_FORMAT_R16G16B16_UNORM:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SNORM:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_USCALED:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SSCALED:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_UINT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SINT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16_SFLOAT:
            result = 6;
            break;
        case VK_FORMAT_R16G16B16A16_UNORM:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SNORM:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_USCALED:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_UINT:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SINT:
            result = 8;
            break;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R32_UINT:
            result = 4;
            break;
        case VK_FORMAT_R32_SINT:
            result = 4;
            break;
        case VK_FORMAT_R32_SFLOAT:
            result = 4;
            break;
        case VK_FORMAT_R32G32_UINT:
            result = 8;
            break;
        case VK_FORMAT_R32G32_SINT:
            result = 8;
            break;
        case VK_FORMAT_R32G32_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R32G32B32_UINT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32_SINT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32_SFLOAT:
            result = 12;
            break;
        case VK_FORMAT_R32G32B32A32_UINT:
            result = 16;
            break;
        case VK_FORMAT_R32G32B32A32_SINT:
            result = 16;
            break;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            result = 16;
            break;
        case VK_FORMAT_R64_UINT:
            result = 8;
            break;
        case VK_FORMAT_R64_SINT:
            result = 8;
            break;
        case VK_FORMAT_R64_SFLOAT:
            result = 8;
            break;
        case VK_FORMAT_R64G64_UINT:
            result = 16;
            break;
        case VK_FORMAT_R64G64_SINT:
            result = 16;
            break;
        case VK_FORMAT_R64G64_SFLOAT:
            result = 16;
            break;
        case VK_FORMAT_R64G64B64_UINT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64_SINT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64_SFLOAT:
            result = 24;
            break;
        case VK_FORMAT_R64G64B64A64_UINT:
            result = 32;
            break;
        case VK_FORMAT_R64G64B64A64_SINT:
            result = 32;
            break;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            result = 32;
            break;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            result = 4;
            break;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            result = 4;
            break;

        default:
            break;
    }
    return result;
}

VkFormat parseFormat(const spirv_cross::SPIRType& type)
{
    if (type.width != 32)
    {
        LOG_FATAL << "Only types with width 32 are supported.";
        return VK_FORMAT_UNDEFINED;
    }
    switch (type.basetype)
    {
        case spirv_cross::SPIRType::Int:
        {
            switch (type.vecsize)
            {
                case 1:
                    return VK_FORMAT_R32_SINT;
                case 2:
                    return VK_FORMAT_R32G32_SINT;
                case 3:
                    return VK_FORMAT_R32G32B32_SINT;
                case 4:
                    return VK_FORMAT_R32G32B32A32_SINT;
                default:
                    LOG_FATAL << "Only types with vecsize 1~4 are supported.";
                    return VK_FORMAT_UNDEFINED;
            }
        }
        case spirv_cross::SPIRType::UInt:
        {
            switch (type.vecsize)
            {
                case 1:
                    return VK_FORMAT_R32_UINT;
                case 2:
                    return VK_FORMAT_R32G32_UINT;
                case 3:
                    return VK_FORMAT_R32G32B32_UINT;
                case 4:
                    return VK_FORMAT_R32G32B32A32_UINT;
                default:
                    LOG_FATAL << "Only types with vecsize 1~4 are supported.";
                    return VK_FORMAT_UNDEFINED;
            }
        }
        case spirv_cross::SPIRType::Float:
        {
            switch (type.vecsize)
            {
                case 1:
                    return VK_FORMAT_R32_SFLOAT;
                case 2:
                    return VK_FORMAT_R32G32_SFLOAT;
                case 3:
                    return VK_FORMAT_R32G32B32_SFLOAT;
                case 4:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
                default:
                    LOG_FATAL << "Only types with vecsize 1~4 are supported.";
                    return VK_FORMAT_UNDEFINED;
            }
        }
        default:
            LOG_FATAL << "Only UInt, Int and Float are supported.";
            return VK_FORMAT_UNDEFINED;
    }
}

ReflectionalSpirV::ReflectionalSpirV(VkDevice device, VkPhysicalDevice physicalDevice)
{
    this->device = device;
    this->physicalDevice = physicalDevice;
    shaderModule = VK_NULL_HANDLE;
    hasDescriptors = false;
}

ReflectionalSpirV::~ReflectionalSpirV()
{
    delete[] stageInfo.pName;

    if (hasDescriptors)
    {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    }
    if (shaderModule)
    {
        vkDestroyShaderModule(device, shaderModule, nullptr);
    }

    delete compiler;
}

VkShaderModule ReflectionalSpirV::GetShaderModule()
{
    return shaderModule;
}

bool ReflectionalSpirV::LoadShaderFile(const boost::container::string& shaderFile)
{
    boost::container::string realShaderFileName = shaderFile + ".spv";
    boost::filesystem::ifstream file(boost::filesystem::path(realShaderFileName.c_str()),
                                     std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        LOG_FATAL << "Failed to open shader file: " << realShaderFileName;
        throw std::runtime_error("Failed to open shader file!");
    }

    transposeMatrix = boost::algorithm::ends_with(shaderFile, ".hlsl");

    size_t fileSize = (size_t) file.tellg();
    boost::container::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    // Create SpirV-Cross compiler module and get shader resources
    compiler = new spirv_cross::CompilerGLSL(reinterpret_cast<uint32_t*>(buffer.data()),
                                             buffer.size() * sizeof(char) / sizeof(uint32_t));

    shaderResources = compiler->get_shader_resources();

    shaderValid = CreateShader();
    if (!shaderValid)
    {
        LOG_ERROR << "Error loading shader " << realShaderFileName << ", shader not valid!";
        return false;
    }

    LOG_INFO << "Shader file " << realShaderFileName << " loaded.";

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create shader module!";
        throw std::runtime_error("Failed to create shader module!");
    }

    stageInfo.module = shaderModule;

    LOG_INFO << "Created shader module.";

    return true;
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

void ReflectionalSpirV::CopyAllBufferData()
{
    // Ensure the shader is valid
    if (!shaderValid) return;

    for (size_t index = 0; index < constantBufferCount; ++index)
    {
        auto currentImage = vulkanBackend->GetCurrentImageIndex();

        void* data;
        vkMapMemory(device, constantBuffersMemory[index][currentImage],
                    0, constantBuffers[index].Size, 0, &data);
        memcpy(data,
               constantBuffers[index].LocalDataBuffer,
               constantBuffers[index].Size);
        vkUnmapMemory(device, constantBuffersMemory[index][currentImage]);
    }
}

void ReflectionalSpirV::CopyBufferData(unsigned int index)
{
    // Ensure the shader is valid
    if (!shaderValid) return;

    // Validate the index
    if (index >= this->constantBufferCount)
        return;

    auto currentImage = vulkanBackend->GetCurrentImageIndex();
    void* data;
    vkMapMemory(device, constantBuffersMemory[index][currentImage],
                0, constantBuffers[index].Size, 0, &data);
    memcpy(data,
           constantBuffers[index].LocalDataBuffer,
           constantBuffers[index].Size);
    vkUnmapMemory(device, constantBuffersMemory[index][currentImage]);
}

void ReflectionalSpirV::CopyBufferData(const boost::container::string& bufferName)
{
    // Ensure the shader is valid
    if (!shaderValid) return;

    // Check for the buffer
    size_t index;
    ReflectionalConstantBuffer* cb = this->FindConstantBuffer(bufferName, &index);
    if (!cb) return;

    auto currentImage = vulkanBackend->GetCurrentImageIndex();
    void* data;
    vkMapMemory(device, constantBuffersMemory[index][currentImage],
                0, cb->Size, 0, &data);
    memcpy(data,
           cb->LocalDataBuffer,
           cb->Size);
    vkUnmapMemory(device, constantBuffersMemory[index][currentImage]);
}

void ReflectionalSpirV::ReleaseConstantBuffer(size_t index)
{
    for (int i = 0; i < vulkanBackend->GetSwapChainImageCount(); ++i)
        vkDestroyBuffer(device, reinterpret_cast<VkBuffer*>(constantBuffers[index].ConstantBuffer)[i], nullptr);

    delete[] reinterpret_cast<VkBuffer*>(constantBuffers[index].ConstantBuffer);
}

bool ReflectionalSpirV::CreateShader()
{
    constantBuffers = new ReflectionalConstantBuffer[shaderResources.uniform_buffers.size()];
    constantBuffersMemory.resize(shaderResources.uniform_buffers.size());
    for (auto& memory : constantBuffersMemory)
        memory.resize(vulkanBackend->GetSwapChainImageCount());
    constantBufferCount = shaderResources.uniform_buffers.size();

    unsigned int b = 0;
    unsigned int setCount = 0;
    //parseUniformBuffers
    for (auto& resource : shaderResources.uniform_buffers)
    {
        constantBuffers[b].Name = compiler->get_name(resource.id).c_str();
        constantBuffers[b].BindIndex = compiler->get_decoration(resource.id, spv::DecorationBinding);
        constantBuffers[b].SetIndex = compiler->get_decoration(resource.id, spv::DecorationDescriptorSet);
        constantBuffers[b].LocationIndex = compiler->get_decoration(resource.id, spv::DecorationLocation);

        cbTable.insert(std::make_pair(constantBuffers[b].Name, constantBuffers + b));

        spirv_cross::SPIRType type = compiler->get_type(resource.base_type_id);

        uint32_t ub_size = 0;
        size_t member_count = type.member_types.size();
        for (uint32_t i = 0; i < member_count; i++)
        {
            ReflectionalShaderVariable ubm{};
            boost::container::string varName = compiler->get_member_name(resource.base_type_id, i).c_str();
//            auto &member_type = glsl.get_type(type.member_types[i]);
//            ubm.type = parseType(member_type);

            ubm.Size = static_cast<unsigned int>(compiler->get_declared_struct_member_size(type, i));
            ubm.ByteOffset = compiler->type_struct_member_offset(type, i);
            ubm.BufferIndex = b;

            ub_size += ubm.Size;

            constantBuffers[b].Variables.push_back(ubm);
            varTable.insert(std::make_pair(varName, ubm));
        }
        constantBuffers[b].Size = ub_size;

        constantBuffers[b].LocalDataBuffer = new unsigned char[constantBuffers[b].Size];
        memset(constantBuffers[b].LocalDataBuffer, 0, constantBuffers[b].Size);

        if (setCount < constantBuffers[b].SetIndex)
        {
            setCount = constantBuffers[b].SetIndex;
        }

        constantBuffers[b].ConstantBuffer = new VkBuffer[vulkanBackend->GetSwapChainImageCount()];

        for (int i = 0; i < vulkanBackend->GetSwapChainImageCount(); ++i)
        {
            createBuffer(constantBuffers[b].Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         reinterpret_cast<VkBuffer*>(constantBuffers[b].ConstantBuffer)[i],
                         constantBuffersMemory[b][i]);
        }


        //parseSampler2Ds
//        for (auto &resource : resources.sampled_images)
//        {
//            Sampler2D s;
//            s.name = glsl.get_name(resource.id);
//            s.set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
//            s.binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
//            if (setCount < s.set)
//            {
//                setCount = s.set;
//            }
//            info->sampler2Ds.push_back(s);
//        }

        ++b;
    }

    createDescriptorSets();
    return true;
}

VkShaderStageFlagBits GetShaderStageFlag(spv::ExecutionModel executionModel)
{
    switch (executionModel)
    {
        case spv::ExecutionModelVertex:
            LOG_DEBUG << "VK_SHADER_STAGE_VERTEX_BIT";
            return VK_SHADER_STAGE_VERTEX_BIT;
        case spv::ExecutionModelTessellationControl:
            LOG_DEBUG << "VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT";
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case spv::ExecutionModelTessellationEvaluation:
            LOG_DEBUG << "VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT";
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case spv::ExecutionModelGeometry:
            LOG_DEBUG << "VK_SHADER_STAGE_GEOMETRY_BIT";
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        case spv::ExecutionModelFragment:
            LOG_DEBUG << "VK_SHADER_STAGE_FRAGMENT_BIT";
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case spv::ExecutionModelGLCompute:
            LOG_DEBUG << "VK_SHADER_STAGE_COMPUTE_BIT";
            return VK_SHADER_STAGE_COMPUTE_BIT;
        case spv::ExecutionModelKernel:
            LOG_DEBUG << "VK_SHADER_STAGE_ALL_GRAPHICS";
            return VK_SHADER_STAGE_ALL_GRAPHICS;
        case spv::ExecutionModelTaskNV:
            LOG_DEBUG << "VK_SHADER_STAGE_TASK_BIT_NV";
            return VK_SHADER_STAGE_TASK_BIT_NV;
        case spv::ExecutionModelMeshNV:
            LOG_DEBUG << "VK_SHADER_STAGE_MESH_BIT_NV";
            return VK_SHADER_STAGE_MESH_BIT_NV;
        case spv::ExecutionModelRayGenerationNV:
            LOG_DEBUG << "VK_SHADER_STAGE_RAYGEN_BIT_NV";
            return VK_SHADER_STAGE_RAYGEN_BIT_NV;
        case spv::ExecutionModelIntersectionNV:
            LOG_DEBUG << "VK_SHADER_STAGE_INTERSECTION_BIT_NV";
            return VK_SHADER_STAGE_INTERSECTION_BIT_NV;
        case spv::ExecutionModelAnyHitNV:
            LOG_DEBUG << "VK_SHADER_STAGE_ANY_HIT_BIT_NV";
            return VK_SHADER_STAGE_ANY_HIT_BIT_NV;
        case spv::ExecutionModelClosestHitNV:
            LOG_DEBUG << "VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV";
            return VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
        case spv::ExecutionModelMissNV:
            LOG_DEBUG << "VK_SHADER_STAGE_MISS_BIT_NV";
            return VK_SHADER_STAGE_MISS_BIT_NV;
        case spv::ExecutionModelCallableNV:
            LOG_DEBUG << "VK_SHADER_STAGE_CALLABLE_BIT_NV";
            return VK_SHADER_STAGE_CALLABLE_BIT_NV;
        case spv::ExecutionModelMax:
            LOG_DEBUG << "VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM";
            return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
    return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

void ReflectionalSpirV::createDescriptorSets()
{
    // Assuming only one entry point in the shader
    auto stages = compiler->get_entry_points_and_stages();
    VkShaderStageFlagBits shaderStageFlag = GetShaderStageFlag(stages[0].execution_model);

    stageInfo = {};

    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = shaderStageFlag;
    stageInfo.pName = new char[stages[0].name.size() + 1];
    memcpy((void*) stageInfo.pName, stages[0].name.c_str(), stages[0].name.size() + 1);


    if (shaderResources.uniform_buffers.empty()) return;

    hasDescriptors = true;

    size_t swapChainImageCount = vulkanBackend->GetSwapChainImageCount();

    // TODO: This assumes that we have only one descriptor!
    boost::container::vector<VkDescriptorPoolSize> poolSize;
    poolSize.resize(1);
    poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize[0].descriptorCount = static_cast<uint32_t>(swapChainImageCount);

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
    poolInfo.pPoolSizes = poolSize.data();
    poolInfo.maxSets = static_cast<uint32_t>(swapChainImageCount);

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create descriptor pool!";
        throw std::runtime_error("Failed to create descriptor pool!");
    }

    boost::container::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;
    uboLayoutBindings.resize(shaderResources.uniform_buffers.size());

    for (size_t i = 0; i < shaderResources.uniform_buffers.size(); ++i)
    {
        LOG_DEBUG << "Shader data:";
        LOG_DEBUG << "\t" << constantBuffers[i].Name << " at"
                  << " set = " << constantBuffers[i].SetIndex
                  << " binding = " << constantBuffers[i].BindIndex
                  << " location = " << constantBuffers[i].LocationIndex;

        uboLayoutBindings[i].binding = compiler->get_decoration(shaderResources.uniform_buffers[i].id,
                                                                spv::DecorationBinding);
        uboLayoutBindings[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBindings[i].descriptorCount = 1;
        uboLayoutBindings[i].stageFlags = shaderStageFlag;
        //uboLayoutBindings[i].stageFlags = VK_SHADER_STAGE_ALL;
        uboLayoutBindings[i].pImmutableSamplers = nullptr; // Optional
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(uboLayoutBindings.size());
    layoutInfo.pBindings = uboLayoutBindings.data();
    // TODO: This still assumes one shader one descriptor set!!!!!!!!
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create descriptor set layout!";
        throw std::runtime_error("Failed to create descriptor set layout!");
    }

    boost::container::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(1);
    for (auto& set : descriptorSets)
    {
        set.resize(swapChainImageCount);
        VkResult res = vkAllocateDescriptorSets(device, &allocInfo, set.data());
        if (res != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to allocate descriptor sets!";
            throw std::runtime_error("Failed to allocate descriptor sets!");
        }
    }

    for (size_t i = 0; i < swapChainImageCount; i++)
    {
        boost::container::vector<VkDescriptorBufferInfo> bufferInfo;
        bufferInfo.resize(uboLayoutBindings.size());
//        bufferInfo.buffer = uniformBuffers[i];
        for (size_t w = 0; w < bufferInfo.size(); ++w)
        {
            bufferInfo[w].buffer = reinterpret_cast<VkBuffer*>(constantBuffers[w].ConstantBuffer)[i];
            bufferInfo[w].offset = 0;
            bufferInfo[w].range = constantBuffers[w].Size;
        }

        boost::container::vector<VkWriteDescriptorSet> descriptorWrite;
        descriptorWrite.resize(uboLayoutBindings.size());

        // TODO: This still assumes that one shader one set!!!!!!
        for (size_t w = 0; w < descriptorWrite.size(); ++w)
        {
            descriptorWrite[w].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[w].dstSet = descriptorSets[0][i];
            descriptorWrite[w].dstBinding = uboLayoutBindings[w].binding;
            descriptorWrite[w].dstArrayElement = 0;

            descriptorWrite[w].descriptorType = uboLayoutBindings[w].descriptorType;
            descriptorWrite[w].descriptorCount = uboLayoutBindings[w].descriptorCount;

            descriptorWrite[w].pBufferInfo = bufferInfo.data();
            descriptorWrite[w].pImageInfo = nullptr; // Optional
            descriptorWrite[w].pTexelBufferView = nullptr; // Optional
        }

        vkUpdateDescriptorSets(device, descriptorWrite.size(), descriptorWrite.data(), 0, nullptr);
    }
}

VkPipelineShaderStageCreateInfo& ReflectionalSpirV::GetStageInfo()
{
    return stageInfo;
}

VkDescriptorSetLayout& ReflectionalSpirV::GetDescriptorSetLayout()
{
    return descriptorSetLayout;
}

bool VertexSpirV::CreateShader()
{
    bool uboResult = ReflectionalSpirV::CreateShader();
    if (!uboResult) return false;

    // Assuming only one entry point in the shader
    auto stages = compiler->get_entry_points_and_stages();
    if (stages[0].execution_model == spv::ExecutionModelVertex)
    {
        // Demonstrates how to generate all necessary data structures to populate
        // a VkPipelineVertexInputStateCreateInfo structure, given the module's
        // expected input variables.
        //
        // Simplifying assumptions:
        // - All vertex input attributes are sourced from a single vertex buffer,
        //   bound to VB slot 0.
        // - Each vertex's attribute are laid out in ascending order by location.
        // - The format of each attribute matches its usage in the shader;
        //   float4 -> VK_FORMAT_R32G32B32A32_FLOAT, etc. No attribute compression is applied.
        // - All attributes are provided per-vertex, not per-instance.
        bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = 0;  // computed below
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
                VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        attributeDescriptions = boost::container::vector<VkVertexInputAttributeDescription>(
                shaderResources.stage_inputs.size(),
                VkVertexInputAttributeDescription{});
        for (size_t i_var = 0; i_var < shaderResources.stage_inputs.size(); ++i_var)
        {
            const spirv_cross::Resource& refl_var = shaderResources.stage_inputs[i_var];
            VkVertexInputAttributeDescription& attr_desc = attributeDescriptions[i_var];
            attr_desc.location = compiler->get_decoration(refl_var.id, spv::DecorationLocation);
            attr_desc.binding = compiler->get_decoration(refl_var.id, spv::DecorationBinding);
            const spirv_cross::SPIRType& type = compiler->get_type(refl_var.base_type_id);
            attr_desc.format = parseFormat(type);
            attr_desc.offset = 0;  // final offset computed below after sorting.
        }
        // Sort attributes by location
        std::sort(std::begin(attributeDescriptions), std::end(attributeDescriptions),
                  [](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b)
                  {
                      return a.location < b.location;
                  });
        // Compute final offsets of each attribute, and total vertex stride.
        for (auto& attribute : attributeDescriptions)
        {
            uint32_t format_size = FormatSize(attribute.format);
            attribute.offset = bindingDescription.stride;
            bindingDescription.stride += format_size;
        }
        // Nothing further is done with attribute_descriptions or binding_description
        // in this sample. A real application would probably derive this information from its
        // mesh format(s); a similar mechanism could be used to ensure mesh/shader compatibility.

        inputInfo = {};
        inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        inputInfo.vertexBindingDescriptionCount = 1;
        inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        inputInfo.pVertexBindingDescriptions = &bindingDescription;
        inputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        return true;
    }
    return false;
}

void VertexSpirV::SetShaderAndCBs()
{
    LOG_FATAL << "Not Implemented";
    throw std::runtime_error("Not Implemented");
}

VertexSpirV::VertexSpirV(VkDevice device, VkPhysicalDevice physicalDevice) : ReflectionalSpirV(device, physicalDevice)
{
}

VertexSpirV::~VertexSpirV()
{

}

VkPipelineVertexInputStateCreateInfo& VertexSpirV::GetInputInfo()
{
    return inputInfo;
}

FragmentSpirV::FragmentSpirV(VkDevice device, VkPhysicalDevice physicalDevice) : ReflectionalSpirV(device,
                                                                                                   physicalDevice)
{

}

FragmentSpirV::~FragmentSpirV()
{

}

bool FragmentSpirV::CreateShader()
{
    bool uboResult = ReflectionalSpirV::CreateShader();
    if (!uboResult) return false;

    // Assuming only one entry point in the shader
    auto stages = compiler->get_entry_points_and_stages();

    return stages[0].execution_model == spv::ExecutionModelFragment;
}

void FragmentSpirV::SetShaderAndCBs()
{

}
