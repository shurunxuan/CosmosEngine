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

    VulkanPipeline(Mesh* mesh, Material* material);

    ~VulkanPipeline() final;

    void RecreatePipeline() final;

    void CreateRenderingPipeline() final;

    VkPipeline& GetPipeline();

private:

    VkPipelineLayout pipelineLayout;

    VkPipeline graphicsPipeline;

    boost::container::vector<VkCommandBuffer> commandBuffers;

};

#endif //GAMEENGINE_VULKANPIPELINE_H
