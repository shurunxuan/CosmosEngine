//
// Created by shuru on 9/7/2019.
//

#ifndef GAMEENGINE_VULKANPIPELINE_H
#define GAMEENGINE_VULKANPIPELINE_H

#include "../RenderingPipeline.h"
#include "ReflectionalSpirV.h"

class ENGINE_LOCAL VulkanPipeline : public RenderingPipeline
{
public:
    friend class VulkanBackend;

    VulkanPipeline();

    ~VulkanPipeline() final;

    void SetVertexShader(const boost::container::string& vs) final;

    void SetPixelShader(const boost::container::string& ps) final;

    void CreateRenderingPipeline() final;

    VkPipeline& GetPipeline();

    void LoadVertexData(void* vertexData, size_t vertexSize, size_t vertexCount) final;

    void LoadIndexData(uint16_t* indexData, size_t indexCount) final;

private:

    void createCommandBuffers();

    VertexSpirV* vertexSpirV;

    FragmentSpirV* fragmentSpirV;

    VkPipelineLayout pipelineLayout;

    VkPipeline graphicsPipeline;

    boost::container::vector<VkCommandBuffer> commandBuffers;

    VkBuffer vertexBuffer;

    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;

    VkDeviceMemory indexBufferMemory;

    uint32_t indicesCount;

};

#endif //GAMEENGINE_VULKANPIPELINE_H
