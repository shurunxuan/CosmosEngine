//
// Created by shuru on 9/7/2019.
//

#include "VulkanPipeline.h"
#include "ReflectionalSpirV.h"
#include "VulkanBackend.h"
#include "../../Logging/Logging.h"

VulkanPipeline::VulkanPipeline(Mesh* mesh, Material* material)
        : RenderingPipeline(mesh, material)
{
    graphicsPipeline = VK_NULL_HANDLE;
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

    boost::container::vector<boost::container::vector<VkDescriptorSet>> transposedDescriptorSets;
    transposedDescriptorSets.resize(vulkanBackend->GetSwapChainImageCount());
    transposedDescriptorSets[0].reserve(descriptorSets.size());
    transposedDescriptorSets[1].reserve(descriptorSets.size());
    transposedDescriptorSets[2].reserve(descriptorSets.size());
    for (int i = 0; i < descriptorSets.size(); ++i)
    {
        transposedDescriptorSets[0].push_back(descriptorSets[i][0]);
        transposedDescriptorSets[1].push_back(descriptorSets[i][1]);
        transposedDescriptorSets[2].push_back(descriptorSets[i][2]);
    }

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

    // Create and Record Command Buffers

    VulkanBufferWithMemory* vBuffer = reinterpret_cast<VulkanBufferWithMemory*>(mesh->GetVertexBuffer());
    VulkanBufferWithMemory* iBuffer = reinterpret_cast<VulkanBufferWithMemory*>(mesh->GetIndexBuffer());

    commandBuffers.resize(vulkanBackend->swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vulkanBackend->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanBackend->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to allocate command buffers!";
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    LOG_INFO << "Allocated command buffers.";

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferInheritanceInfo inheritanceInfo = {};
        inheritanceInfo.renderPass = vulkanBackend->renderPass;
        inheritanceInfo.framebuffer = vulkanBackend->swapChainFramebuffers[i];

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        beginInfo.pInheritanceInfo = &inheritanceInfo; // Optional

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to begin recording command buffer " << i;
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {vBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], iBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(transposedDescriptorSets[i].size()),
                                transposedDescriptorSets[i].data(), 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], mesh->GetIndexCount(), 1, 0, 0, 0);

        //        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to record command buffer!";
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
}

VkPipeline& VulkanPipeline::GetPipeline()
{
    return graphicsPipeline;
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

    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool,
                         static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

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

}

void VulkanPipeline::createDescriptorSets()
{
    uint32_t swapChainImageCount = static_cast<uint32_t>(vulkanBackend->GetSwapChainImageCount());


    for (auto& itr : setBindingsLayoutMap)
    {
        boost::container::vector<VkDescriptorPoolSize> poolSize;
        poolSize.reserve(uniformBuffers.size() + textureViews.size() + samplers.size());

        for (size_t i = 0; i < uniformBuffers.size(); ++i)
        {
            if (uniformBuffers[i]->SetIndex != itr.first)
                continue;
            VkDescriptorPoolSize p = {};
            p.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            p.descriptorCount = swapChainImageCount;
            poolSize.push_back(p);
        }
        for (size_t i = 0; i < textureViews.size(); ++i)
        {
            if (textureViews[i]->SetIndex != itr.first)
                continue;
            VkDescriptorPoolSize p = {};
            p.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            p.descriptorCount = swapChainImageCount;
            poolSize.push_back(p);
        }
        for (size_t i = 0; i < samplers.size(); ++i)
        {
            if (samplers[i]->SetIndex != itr.first)
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
            // TODO: Samplers / Textures
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

            // Notice for the above todo: this already takes other descriptors into consideration.
            // but not the count (descriptorWrite.size())
//            descriptorWrite.resize(itr.second.size());

            vkUpdateDescriptorSets(vulkanBackend->device, static_cast<uint32_t>(descriptorWrite.size()),
                                   descriptorWrite.data(), 0, nullptr);
        }

    }

}


