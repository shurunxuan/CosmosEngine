//
// Created by shuru on 7/13/2019.
//

#include "VulkanBackend.h"
#include "../../Logging/Logging.h"

#include <optional>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value();
    }
};

const boost::container::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

bool enableValidationLayers =
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
    pickPhysicalDevice();
    createLogicalDevice();

    return true;
}

void VulkanBackend::DeInit()
{
    vkDestroyDevice(device, nullptr);

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
        LOG_WARNING << "Validation layers requested, but not available! Disabling validation layers.";
        enableValidationLayers = false;
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
    if (enableValidationLayers)
    {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else
    {
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create vulkan instance!";
        throw std::runtime_error("Failed to create vulkan instance!");
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
        LOG_FATAL << "Failed to set up debug messenger!";
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

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

int rateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    LOG_INFO << "Device " << deviceProperties.deviceID << ": ";
    LOG_INFO << "\tName: " << deviceProperties.deviceName;
    switch (deviceProperties.deviceType)
    {

        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_OTHER";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_CPU";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_RANGE_SIZE";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
            LOG_INFO << "\tType: VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM";
            break;
    }

    int score = 0;

    QueueFamilyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete())
    {
        return -1;
    }

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    else
    {
        score += 1;
    }

    // TODO: Choose device with capabilities and features

    return score;
}

void VulkanBackend::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        LOG_FATAL << "Failed to find GPUs with Vulkan support!";
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices)
    {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0)
    {
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        LOG_FATAL << "Failed to find a suitable GPU!";
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void VulkanBackend::createLogicalDevice()
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create logical device!";
        throw std::runtime_error("Failed to create logical device!");
    }
    else
    {
        LOG_INFO << "Created logical device with " << deviceProperties.deviceName << ".";
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}