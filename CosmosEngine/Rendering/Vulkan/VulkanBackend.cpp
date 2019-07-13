//
// Created by shuru on 7/13/2019.
//

#include "VulkanBackend.h"
#include "../../Logging/Logging.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

VulkanBackend::VulkanBackend()
{

}

VulkanBackend::~VulkanBackend()
{

}

bool VulkanBackend::StartUp()
{
    LOG_INFO << "Initializing Vulkan Rendering Backend";

    const auto enumerate_instance_version =
            reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr,
                                                                                   "vkEnumerateInstanceVersion"));

    uint32_t instance_version = VK_API_VERSION_1_0;

    if (enumerate_instance_version != nullptr)
    {
        enumerate_instance_version(&instance_version);
    }

    const uint32_t vulkan_major = VK_VERSION_MAJOR(instance_version);
    const uint32_t vulkan_minor = VK_VERSION_MINOR(instance_version);
    const uint32_t vulkan_patch = VK_VERSION_PATCH(VK_HEADER_VERSION);

    LOG_INFO << "Vulkan Instance Version: "
              << vulkan_major << "."
              << vulkan_minor << "."
              << vulkan_patch;

    initVulkan();

    return true;
}

void VulkanBackend::Shutdown()
{
    vkDestroyInstance(vulkanInstance, nullptr);
}

void VulkanBackend::Update()
{

}

void VulkanBackend::initVulkan()
{
    // Create vulkan instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }

    // Enumerate extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    LOG_INFO << extensionCount << " extensions supported";

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    LOG_INFO << "available extensions:";

    for (const auto& extension : extensions)
    {
        std::stringstream ss;
        ss << "  ";
        bool required = false;
        for (unsigned int i = 0; i < glfwExtensionCount; ++i)
        {
            if (strcmp(extension.extensionName, glfwExtensions[i]) == 0)
            {
                required = true;
                break;
            }
        }
        if (required)
        {
            ss << "* ";
        }
        else
        {
            ss << "  ";
        }
        ss << extension.extensionName;

        LOG_INFO << ss.str();
    }
}