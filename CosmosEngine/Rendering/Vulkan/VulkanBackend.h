//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_VULKANBACKEND_H
#define COSMOSENGINE_VULKANBACKEND_H

#include "../../Export.h"

#include "../RenderingBackend.h"

#include <boost/container/vector.hpp>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

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

    void pickPhysicalDevice();

    VkInstance vulkanInstance{};

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};


#endif //COSMOSENGINE_VULKANBACKEND_H
