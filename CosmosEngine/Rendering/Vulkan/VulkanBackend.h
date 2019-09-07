//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_VULKANBACKEND_H
#define COSMOSENGINE_VULKANBACKEND_H

#include "../../Export.h"

#include "../RenderingBackend.h"
#include "ReflectionalSpirV.h"

#include <optional>
#include <boost/container/vector.hpp>
#include <boost/array.hpp>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct ENGINE_LOCAL QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

struct ENGINE_LOCAL SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    boost::container::vector<VkSurfaceFormatKHR> formats;
    boost::container::vector<VkPresentModeKHR> presentModes;
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
};

const std::vector<Vertex> vertices = {
        {{-0.5f, 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f,  0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f,  0.0f, 0.5f},  {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.0f, 0.5f},  {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
        0, 2, 1, 2, 0, 3, 0, 1, 2, 2, 3, 0
};

struct UniformBufferObject
{
    glm::mat4x4 model;
    glm::mat4x4 view;
    glm::mat4x4 proj;
};

class ENGINE_API VulkanBackend final
        : public RenderingBackend
{
public:
    friend class ReflectionalSpirV;

    VulkanBackend();

    ~VulkanBackend() final;

    bool Init() override;

    void Render(float deltaTime, float totalTime) override;

    void DeInit() override;

    boost::container::vector<char> loadShader(const boost::container::string& filename) override;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);

    size_t GetSwapChainImageCount();

    size_t GetCurrentFrame();

    uint32_t GetCurrentImageIndex();

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

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    VkShaderModule createShaderModule(const boost::container::vector<char>& code);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

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

    uint32_t imageIndex = 0;

    VkBuffer vertexBuffer;

    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;

    VkDeviceMemory indexBufferMemory;

    boost::container::vector<VkBuffer> uniformBuffers;

    boost::container::vector<VkDeviceMemory> uniformBuffersMemory;

    VertexSpirV* testVertexSpirV;
    FragmentSpirV* testFragmentSpirV;
};


extern ENGINE_LOCAL VulkanBackend* vulkanBackend;

#endif //COSMOSENGINE_VULKANBACKEND_H
