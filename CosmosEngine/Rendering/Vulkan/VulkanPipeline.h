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
    void cleanup();

    void buildLayoutInfo(ReflectionalSpirV* shader);

    void createDescriptorSets();

    VkPipelineLayout pipelineLayout;

    VkPipeline graphicsPipeline;

    boost::container::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    boost::container::map<uint32_t, boost::container::vector<VkDescriptorSetLayoutBinding>> setBindingsLayoutMap;
    boost::container::vector<ReflectionalConstantBuffer*> uniformBuffers;
    boost::container::vector<VkDescriptorPool> descriptorPools;
    boost::container::vector<VkDescriptorSetLayout> setLayouts;

    boost::container::vector<VkCommandBuffer> commandBuffers;
    boost::container::vector<boost::container::vector<VkDescriptorSet>> descriptorSets;

};

#endif //GAMEENGINE_VULKANPIPELINE_H
