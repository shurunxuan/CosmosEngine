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

struct ENGINE_LOCAL SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    boost::container::vector<VkSurfaceFormatKHR> formats;
    boost::container::vector<VkPresentModeKHR> presentModes;
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

    boost::container::vector<char> loadShader(const boost::container::string& filename) override;
private:
    void createInstance();

    void setupDebugMessenger();

    bool checkValidationLayerSupport();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    int rateDeviceSuitability(VkPhysicalDevice device);

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSurface();

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

    void recreateSwapChain();

    void cleanupSwapChain();

    VkShaderModule createShaderModule(const boost::container::vector<char>& code);

    VkInstance vulkanInstance{};

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    VkQueue graphicsQueue;

    VkQueue presentQueue;

    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain;

    boost::container::vector<VkImage> swapChainImages;

    boost::container::vector<VkImageView> swapChainImageViews;

    VkFormat swapChainImageFormat;

    VkExtent2D swapChainExtent;

    VkRenderPass renderPass;

    VkPipelineLayout pipelineLayout;

    VkPipeline graphicsPipeline;

    boost::container::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;

    boost::container::vector<VkCommandBuffer> commandBuffers;

    boost::container::vector<VkSemaphore> imageAvailableSemaphores;

    boost::container::vector<VkSemaphore> renderFinishedSemaphores;

    boost::container::vector<VkFence> inFlightFences;

    size_t currentFrame = 0;


};


#endif //COSMOSENGINE_VULKANBACKEND_H
