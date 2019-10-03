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

    friend class VulkanCommandBuffer;

    VulkanPipeline(Material* material);

    ~VulkanPipeline() final;

    void RecreatePipeline() final;

    void CreateRenderingPipeline() final;

    VkPipeline& GetPipeline();

    VkPipelineLayout& GetPipelineLayout();

    bool SetTexture(const boost::container::string& name, const Texture& texture) final;

    bool SetSampler(const boost::container::string& name, const Texture& texture) final;

    bool SetSamplerTexture(const boost::container::string& samplerName, const boost::container::string& textureName,
                           const Texture& texture);
private:
    void cleanup();

    void buildLayoutInfo(ReflectionalSpirV* shader);

    void createDescriptorSets();

    bool recreated;

    VkPipelineLayout pipelineLayout;

    VkPipeline graphicsPipeline;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    boost::container::map<uint32_t, boost::container::vector<VkDescriptorSetLayoutBinding>> setBindingsLayoutMap;
    boost::container::vector<ReflectionalConstantBuffer*> uniformBuffers;
    boost::container::vector<ReflectionalTextureView*> textureViews;
    boost::container::vector<ReflectionalSampler*> samplers;
    boost::container::vector<VkDescriptorPool> descriptorPools;
    boost::container::vector<VkDescriptorSetLayout> setLayouts;

    boost::container::vector<boost::container::vector<VkDescriptorSet>> descriptorSets;

};
#endif //GAMEENGINE_VULKANPIPELINE_H
