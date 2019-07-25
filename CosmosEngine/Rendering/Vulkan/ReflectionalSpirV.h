//
// Created by shuru on 7/15/2019.
//

#ifndef GAMEENGINE_REFLECTIONALSPIRV_H
#define GAMEENGINE_REFLECTIONALSPIRV_H

#include "../ReflectionalShader.h"

#include <vulkan/vulkan.h>

class ReflectionalSpirV : virtual public ReflectionalShader
{
public:
    explicit ReflectionalSpirV(VkDevice device, VkPhysicalDevice physicalDevice);
    virtual ~ReflectionalSpirV();

    // Initialization method (since we can't invoke derived class
    // overrides in the base class constructor)
    bool LoadShaderFile(const boost::container::string& shaderFile) override;

    // Activating the shader and copying data
    void CopyAllBufferData();
    void CopyBufferData(unsigned int index);
    void CopyBufferData(const boost::container::string& bufferName);

    // Misc getters
    VkShaderModule GetShaderModule();

protected:
    VkShaderModule shaderModule;
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    std::vector<VkDeviceMemory> constantBuffersMemory;

    virtual bool CreateShader(const std::vector<uint32_t>& shaderBinary) = 0;
    void ReleaseConstantBuffer(size_t index) override;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif //GAMEENGINE_REFLECTIONALSPIRV_H
