//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_VULKANBACKEND_H
#define COSMOSENGINE_VULKANBACKEND_H

#include "../../Export.h"

#include "../RenderingBackend.h"

#include <optional>
#include <boost/container/vector.hpp>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

struct ENGINE_LOCAL QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

class ENGINE_API VulkanBackend final
        : public RenderingBackend
{
public:
    VulkanBackend();

    ~VulkanBackend() final;

    bool Init() override;

    void Render() override;

    void DeInit() override;

private:
    void createInstance();

    void setupDebugMessenger();

    bool checkValidationLayerSupport();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    int rateDeviceSuitability(VkPhysicalDevice device);

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSurface();

    VkInstance vulkanInstance{};

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    VkQueue graphicsQueue;

    VkQueue presentQueue;

    VkSurfaceKHR surface;
};


#endif //COSMOSENGINE_VULKANBACKEND_H
