//
// Created by shuru on 7/15/2019.
//

#ifndef GAMEENGINE_REFLECTIONALSPIRV_H
#define GAMEENGINE_REFLECTIONALSPIRV_H

#include "../ReflectionalShader.h"

#include <vulkan/vulkan.h>
#include "SPIRV_Reflect/spirv_reflect.h"

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

    SpvReflectShaderModule module;

    std::vector<VkDeviceMemory> constantBuffersMemory;

    virtual bool CreateShader(const boost::container::vector<uint32_t>& shaderBinary) = 0;

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
    bool CreateShader(const boost::container::vector<uint32_t>& shaderBinary) override;

    void SetShaderAndCBs() final;

    VkVertexInputBindingDescription bindingDescription;

    boost::container::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

#endif //GAMEENGINE_REFLECTIONALSPIRV_H
