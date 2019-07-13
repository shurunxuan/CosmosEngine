//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_VULKANBACKEND_H
#define COSMOSENGINE_VULKANBACKEND_H

#include "../../Export.h"

#include "../RenderingBackend.h"

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

class ENGINE_API VulkanBackend final
        : public RenderingBackend
{
public:
    VulkanBackend();

    ~VulkanBackend() final;

    bool StartUp() override;

    void Update() override;

    void Shutdown() override;

private:
    void initVulkan();


    VkInstance vulkanInstance{};
};


#endif //COSMOSENGINE_VULKANBACKEND_H
