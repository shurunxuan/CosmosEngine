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

struct VulkanBufferWithMemory
{
    VkBuffer buffer;
    VkDeviceMemory deviceMemory;
};

class ENGINE_API VulkanBackend final
        : public RenderingBackend
{
public:
    friend class ReflectionalSpirV;
    friend class VulkanPipeline;

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

    RenderingPipeline* CreateRenderingPipeline(Mesh* mesh, Material* material) final;

    void DestroyRenderingPipeline(RenderingPipeline** pipeline) final;

    ReflectionalShader* CreateVertexShader(const boost::container::string& filename) final;

    void DestroyVertexShader(ReflectionalShader** shader) final;

    ReflectionalShader* CreatePixelShader(const boost::container::string& filename) final;

    void DestroyPixelShader(ReflectionalShader** shader) final;

    void* CreateVertexBuffer(void* vertexData, size_t vertexSize, size_t vertexCount) final;

    void DestroyVertexBuffer(void** vertexBuffer) final;

    void* CreateIndexBuffer(uint16_t* indexData, size_t indexCount) final;

    void DestroyIndexBuffer(void** indexBuffer) final;

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

    void createFramebuffers();

    void createCommandPool();

    void createSyncObjects();

    void recreateSwapChain();

    void cleanupSwapChain();

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

    boost::container::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;

    boost::container::vector<VkSemaphore> imageAvailableSemaphores;

    boost::container::vector<VkSemaphore> renderFinishedSemaphores;

    boost::container::vector<VkFence> inFlightFences;

    size_t currentFrame = 0;

    uint32_t imageIndex = 0;
};


extern ENGINE_LOCAL VulkanBackend* vulkanBackend;

#endif //COSMOSENGINE_VULKANBACKEND_H
