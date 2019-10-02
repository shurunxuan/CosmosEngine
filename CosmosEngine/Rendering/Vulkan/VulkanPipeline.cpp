//
// Created by shuru on 9/7/2019.
//

#include "VulkanPipeline.h"
#include "ReflectionalSpirV.h"
#include "VulkanBackend.h"
#include "../../Logging/Logging.h"
#include "../../Core/Texture.h"

VulkanPipeline::VulkanPipeline(Material* material)
        : RenderingPipeline(material)
{
    graphicsPipeline = VK_NULL_HANDLE;
    recreated = false;
}

VulkanPipeline::~VulkanPipeline()
{
    for (auto& shaderStage : shaderStages)
    {
        delete[] shaderStage.pName;
    }

    cleanup();
}

void VulkanPipeline::CreateRenderingPipeline()
{
    VertexSpirV* vertexSpirV = dynamic_cast<VertexSpirV*>(material->GetVertexShader());
    FragmentSpirV* fragmentSpirV = dynamic_cast<FragmentSpirV*>(material->GetPixelShader());

    buildLayoutInfo(vertexSpirV);
    buildLayoutInfo(fragmentSpirV);

    createDescriptorSets();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vulkanBackend->swapChainExtent.width;
    viewport.height = (float) vulkanBackend->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = vulkanBackend->swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional


    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    //setLayouts.clear();
    //descriptorSets.clear();
    //descriptorSets.resize(vulkanBackend->swapChainImages.size());
    //for (size_t i = 0; i < vulkanBackend->swapChainImages.size(); ++i)
    //{
    //    if (vertexSpirV->hasDescriptors)
    //    {
    //        //setLayouts.push_back(vertexSpirV->GetDescriptorSetLayout());
    //        for (auto set : vertexSpirV->descriptorSets)
    //        {
    //            descriptorSets[i].push_back(set[i]);
    //        }
    //    }

    //    if (fragmentSpirV->hasDescriptors)
    //    {
    //        //setLayouts.push_back(fragmentSpirV->GetDescriptorSetLayout());
    //        for (auto set : fragmentSpirV->descriptorSets)
    //        {
    //            descriptorSets[i].push_back(set[i]);
    //        }
    //    }
    //}

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    pipelineLayoutInfo.pSetLayouts = setLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(vulkanBackend->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create pipeline layout!";
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    LOG_INFO << "Created pipeline layout.";

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexSpirV->GetInputInfo();
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = vulkanBackend->renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    auto result = vkCreateGraphicsPipelines(vulkanBackend->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                            &graphicsPipeline);
    if (result != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create graphics pipeline!";
        throw std::runtime_error("Failed to create graphics pipeline!");
    }

    LOG_INFO << "Created graphics pipeline.";


}

VkPipeline& VulkanPipeline::GetPipeline()
{
    return graphicsPipeline;
}

VkPipelineLayout& VulkanPipeline::GetPipelineLayout()
{
    return pipelineLayout;
}

void VulkanPipeline::cleanup()
{
    setBindingsLayoutMap.clear();
    shaderStages.clear();

    for (auto& descriptorSet : descriptorSets)
        descriptorSet.clear();
    descriptorSets.clear();

    uniformBuffers.clear();
    textureViews.clear();
    samplers.clear();

    varTable.clear();

    vkDestroyPipeline(vulkanBackend->device, graphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vulkanBackend->device, pipelineLayout, nullptr);

    for (auto& descriptorPool : descriptorPools)
        vkDestroyDescriptorPool(vulkanBackend->device, descriptorPool, nullptr);
    descriptorPools.clear();

    for (auto& setLayout : setLayouts)
        vkDestroyDescriptorSetLayout(vulkanBackend->device, setLayout, nullptr);
    setLayouts.clear();
}

void VulkanPipeline::RecreatePipeline()
{
    cleanup();

    CreateRenderingPipeline();

    recreated = true;
}


void VulkanPipeline::buildLayoutInfo(ReflectionalSpirV* shader)
{
    // Assuming only one entry point in the shader
    auto stages = shader->compiler->get_entry_points_and_stages();
    VkShaderStageFlagBits shaderStageFlag = GetShaderStageFlag(stages[0].execution_model);

    VkPipelineShaderStageCreateInfo stageInfo = {};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = shaderStageFlag;
    stageInfo.pName = new char[stages[0].name.size() + 1];
    stageInfo.module = shader->GetShaderModule();
    memcpy((void*) stageInfo.pName, stages[0].name.c_str(), stages[0].name.size() + 1);

    shaderStages.push_back(stageInfo);

    if (shader->shaderResources.uniform_buffers.empty() &&
        shader->shaderResources.separate_images.empty() &&
        shader->shaderResources.separate_samplers.empty())
        return;

    for (auto& itr : shader->setBindingsLayoutMap)
    {
        auto findResult = setBindingsLayoutMap.find(itr.first);
        if (findResult == setBindingsLayoutMap.end())
        {
            setBindingsLayoutMap.insert(std::make_pair(itr.first, itr.second));
        }
        else
        {
            for (auto& b : itr.second)
            {
                findResult->second.push_back(b);
            }
        }
    }

    for (size_t i = 0; i < shader->GetBufferCount(); ++i)
    {
        uniformBuffers.push_back(&(shader->constantBuffers[i]));
    }

    for (size_t i = 0; i < shader->GetTextureViewCount(); ++i)
    {
        textureViews.push_back(shader->textureViews[i]);
    }

    for (size_t i = 0; i < shader->GetSamplerCount(); ++i)
    {
        samplers.push_back(shader->samplerStates[i]);
    }

    for (auto& var : shader->varTable)
    {
        varTable.insert(std::make_pair(var.first, shader));
    }

    for (auto& var : shader->textureTable)
    {
        imageTable.insert(std::make_pair(var.first, shader));
    }

    for (auto& var : shader->samplerTable)
    {
        samplerTable.insert(std::make_pair(var.first, shader));
    }

}

void VulkanPipeline::createDescriptorSets()
{
    uint32_t swapChainImageCount = static_cast<uint32_t>(vulkanBackend->GetSwapChainImageCount());


    for (auto& itr : setBindingsLayoutMap)
    {
        boost::container::vector<VkDescriptorPoolSize> poolSize;
        poolSize.reserve(uniformBuffers.size() + textureViews.size() + samplers.size());

        for (auto& uniformBuffer : uniformBuffers)
        {
            if (uniformBuffer->SetIndex != itr.first)
                continue;
            VkDescriptorPoolSize p = {};
            p.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            p.descriptorCount = swapChainImageCount;
            poolSize.push_back(p);
        }
        for (auto& textureView : textureViews)
        {
            if (textureView->SetIndex != itr.first)
                continue;
            VkDescriptorPoolSize p = {};
            p.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            p.descriptorCount = swapChainImageCount;
            poolSize.push_back(p);
        }
        for (auto& sampler : samplers)
        {
            if (sampler->SetIndex != itr.first)
                continue;
            VkDescriptorPoolSize p = {};
            p.type = VK_DESCRIPTOR_TYPE_SAMPLER;
            p.descriptorCount = swapChainImageCount;
            poolSize.push_back(p);
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
        poolInfo.pPoolSizes = poolSize.data();
        poolInfo.maxSets = swapChainImageCount;

        VkDescriptorPool descriptorPool;

        if (vkCreateDescriptorPool(vulkanBackend->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to create descriptor pool!";
            throw std::runtime_error("Failed to create descriptor pool!");
        }

        descriptorPools.push_back(descriptorPool);

        VkDescriptorSetLayout descriptorSetLayout;

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(itr.second.size());
        layoutInfo.pBindings = itr.second.data();

        if (vkCreateDescriptorSetLayout(vulkanBackend->device, &layoutInfo, nullptr, &descriptorSetLayout) !=
            VK_SUCCESS)
        {
            LOG_FATAL << "Failed to create descriptor set layout!";
            throw std::runtime_error("Failed to create descriptor set layout!");
        }

        setLayouts.push_back(descriptorSetLayout);

        boost::container::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = swapChainImageCount;
        allocInfo.pSetLayouts = layouts.data();

        //descriptorSets.resize(1);
        boost::container::vector<VkDescriptorSet> set;

        set.resize(swapChainImageCount);
        if (vkAllocateDescriptorSets(vulkanBackend->device, &allocInfo, set.data()) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to allocate descriptor sets!";
            throw std::runtime_error("Failed to allocate descriptor sets!");
        }

        descriptorSets.push_back(set);

        for (size_t i = 0; i < swapChainImageCount; i++)
        {
            boost::container::vector<VkDescriptorBufferInfo> bufferInfo;
            bufferInfo.reserve(uniformBuffers.size());
            boost::container::vector<VkDescriptorImageInfo> imageInfo;
            imageInfo.reserve(textureViews.size() + samplers.size());
            std::vector<VkWriteDescriptorSet> descriptorWrite;
            //        bufferInfo.buffer = uniformBuffers[i];
            for (auto& uniformBuffer : uniformBuffers)
            {
                if (uniformBuffer->SetIndex != itr.first)
                    continue;
                VkDescriptorBufferInfo info = {};

                info.buffer = reinterpret_cast<VkBuffer*>(uniformBuffer->ConstantBuffer)[i];
                info.offset = 0;
                info.range = uniformBuffer->Size;
                bufferInfo.push_back(info);

                VkWriteDescriptorSet d = {};
                d.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                d.dstSet = set[i];
                d.dstBinding = uniformBuffer->BindIndex;
                d.dstArrayElement = 0;

                d.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                d.descriptorCount = 1;

                d.pBufferInfo = &bufferInfo.back();
                d.pImageInfo = nullptr; // Optional
                d.pTexelBufferView = nullptr; // Optional

                descriptorWrite.push_back(d);
            }
            for (auto& texture : textureViews)
            {
                if (texture->SetIndex != itr.first)
                    continue;
                VkDescriptorImageInfo info = {};
                info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                if (texture->data == nullptr)
                    info.imageView = vulkanBackend->nullImageView;
                else
                    info.imageView = VkImageView(texture->data);
                info.sampler = nullptr;
                imageInfo.push_back(info);

                VkWriteDescriptorSet d = {};
                d.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                d.dstSet = set[i];
                d.dstBinding = texture->BindIndex;
                d.dstArrayElement = 0;

                d.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                d.descriptorCount = 1;

                d.pBufferInfo = nullptr;
                d.pImageInfo = &imageInfo.back(); // Optional
                d.pTexelBufferView = nullptr; // Optional

                descriptorWrite.push_back(d);
            }

            for (auto& sampler : samplers)
            {
                if (sampler->SetIndex != itr.first)
                    continue;
                VkDescriptorImageInfo info = {};
                //info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                info.imageView = nullptr;
                if (sampler->data == nullptr)
                    info.sampler = vulkanBackend->nullSampler;
                else
                    info.sampler = VkSampler(sampler->data);
                imageInfo.push_back(info);

                VkWriteDescriptorSet d = {};
                d.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                d.dstSet = set[i];
                d.dstBinding = sampler->BindIndex;
                d.dstArrayElement = 0;

                d.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                d.descriptorCount = 1;

                d.pBufferInfo = nullptr;
                d.pImageInfo = &imageInfo.back(); // Optional
                d.pTexelBufferView = nullptr; // Optional

                descriptorWrite.push_back(d);
            }
            vkUpdateDescriptorSets(vulkanBackend->device, static_cast<uint32_t>(descriptorWrite.size()),
                                   descriptorWrite.data(), 0, nullptr);
        }

    }

}

bool VulkanPipeline::SetTexture(const boost::container::string& name, const Texture& texture)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            imageTable.find(name);

    // Did we find the key?
    if (result == imageTable.end())
        return false;

    bool r = result->second->SetImage(name,
                                      reinterpret_cast<VulkanTextureData*>(texture.GetTextureData())->textureImageView);

    RecreatePipeline();

    return r;
}

bool VulkanPipeline::SetSampler(const boost::container::string& name, const Texture& texture)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            samplerTable.find(name);

    // Did we find the key?
    if (result == samplerTable.end())
        return false;

    bool r = result->second->SetSampler(name, texture.GetSampler());

    RecreatePipeline();

    return r;
}

bool VulkanPipeline::SetSamplerTexture(const boost::container::string& samplerName,
                                       const boost::container::string& textureName, const Texture& texture)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            samplerTable.find(samplerName);

    // Did we find the key?
    if (result == samplerTable.end())
        return false;

    bool r = result->second->SetSampler(samplerName, texture.GetSampler());

    if (!r) return false;

    // Look for the key
    result = imageTable.find(textureName);

    // Did we find the key?
    if (result == imageTable.end())
        return false;

    r = result->second->SetImage(textureName,
                                 reinterpret_cast<VulkanTextureData*>(texture.GetTextureData())->textureImageView);

    if (!r) return false;

    RecreatePipeline();

    return r;
}



