//
// Created by shuru on 7/15/2019.
//

#ifndef GAMEENGINE_REFLECTIONALSPIRV_H
#define GAMEENGINE_REFLECTIONALSPIRV_H

#include "../ReflectionalShader.h"

#include <vulkan/vulkan.h>
#include <spirv_cross/spirv_glsl.hpp>

class ReflectionalSpirV : virtual public ReflectionalShader
{
public:
    explicit ReflectionalSpirV(VkDevice device, VkPhysicalDevice physicalDevice);

    virtual ~ReflectionalSpirV();

    // Initialization method (since we can't invoke derived class
    // overrides in the base class constructor)
    bool LoadShaderFile(const boost::container::string& shaderFile) override;

    // Activating the shader and copying data
    void CopyAllBufferData() override;

    void CopyBufferData(unsigned int index) override;

    void CopyBufferData(const boost::container::string& bufferName) override;

    // Misc getters
    VkShaderModule GetShaderModule();

protected:
    VkShaderModule shaderModule;
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    spirv_cross::CompilerGLSL* compiler = nullptr;
    spirv_cross::ShaderResources shaderResources;
    boost::container::vector<boost::container::vector<VkDeviceMemory>> constantBuffersMemory;

    virtual bool CreateShader();

    void ReleaseConstantBuffer(size_t index) override;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

class VertexSpirV : public ReflectionalSpirV
{
public:
    friend class VulkanBackend;

    VertexSpirV(VkDevice device, VkPhysicalDevice physicalDevice);

    ~VertexSpirV() final;

private:
    bool CreateShader() override;

    void SetShaderAndCBs() final;

    VkVertexInputBindingDescription bindingDescription;

    boost::container::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

class FragmentSpirV : public ReflectionalSpirV
{
public:
    friend class VulkanBackend;

    FragmentSpirV(VkDevice device, VkPhysicalDevice physicalDevice);

    ~FragmentSpirV() final;

private:
    bool CreateShader() override;

    void SetShaderAndCBs() final;
};

#endif //GAMEENGINE_REFLECTIONALSPIRV_H
