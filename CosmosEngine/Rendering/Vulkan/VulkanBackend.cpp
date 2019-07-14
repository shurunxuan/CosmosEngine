//
// Created by shuru on 7/13/2019.
//

#include "VulkanBackend.h"
#include "../../Logging/Logging.h"


const boost::container::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

const bool enableValidationLayers =
#ifdef NDEBUG
        false;
#else
        true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        LOG_ERROR << "Validation layer: " << pCallbackData->pMessage;
    }
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LOG_WARNING << "Validation layer: " << pCallbackData->pMessage;
    }
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        LOG_INFO << "Validation layer: " << pCallbackData->pMessage;
    }
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        LOG_TRACE << "Validation layer: " << pCallbackData->pMessage;
    }

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

VulkanBackend::VulkanBackend()
{

}

VulkanBackend::~VulkanBackend()
{

}

bool VulkanBackend::Init()
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

    createInstance();
    setupDebugMessenger();

    return true;
}

void VulkanBackend::DeInit()
{
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(vulkanInstance, debugMessenger, nullptr);
    }

    vkDestroyInstance(vulkanInstance, nullptr);
}

void VulkanBackend::Render()
{

}

std::vector<const char*> getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void VulkanBackend::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    // Create vulkan instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "CosmosEngine Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto requiredExtensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }

    // Enumerate extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    LOG_INFO << extensionCount << " extensions supported";

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    LOG_INFO << "Available extensions:";

    for (const auto& extension : availableExtensions)
    {
        std::stringstream ss;
        ss << "  ";
        bool required = false;
        for (auto requiredExtension : requiredExtensions)
        {
            if (strcmp(extension.extensionName, requiredExtension) == 0)
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

void VulkanBackend::setupDebugMessenger()
{
    if (!enableValidationLayers)
    { return; }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

bool VulkanBackend::checkValidationLayerSupport()
{
    // Validation layers
    if (!enableValidationLayers)
    {
        LOG_INFO << "Compiled in Release mode, disabling validation layers";
    }
    else
    {
        LOG_INFO << "Compiled in Debug mode, enabling validation layers";
    }

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;
        std::stringstream ss;
        ss << "  ";

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}
