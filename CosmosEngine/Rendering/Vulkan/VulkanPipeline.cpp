//
// Created by shuru on 9/7/2019.
//

#include "VulkanPipeline.h"
#include "ReflectionalSpirV.h"
#include "VulkanBackend.h"
#include "../../Logging/Logging.h"

VulkanPipeline::VulkanPipeline()
        : RenderingPipeline()
{
    vertexSpirV = nullptr;
    fragmentSpirV = nullptr;
	graphicsPipeline = VK_NULL_HANDLE;
}

VulkanPipeline::~VulkanPipeline()
{
    vkDestroyBuffer(vulkanBackend->device, indexBuffer, nullptr);
    vkFreeMemory(vulkanBackend->device, indexBufferMemory, nullptr);

    vkDestroyBuffer(vulkanBackend->device, vertexBuffer, nullptr);
    vkFreeMemory(vulkanBackend->device, vertexBufferMemory, nullptr);

    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(vulkanBackend->device, graphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vulkanBackend->device, pipelineLayout, nullptr);

    delete vertexSpirV;

    delete fragmentSpirV;
}

void VulkanPipeline::SetVertexShader(const boost::container::string& vs)
{
    vertexSpirV = new VertexSpirV(vulkanBackend->device, vulkanBackend->physicalDevice);
    vertexSpirV->LoadShaderFile(vs);
}

void VulkanPipeline::SetPixelShader(const boost::container::string& ps)
{
    fragmentSpirV = new FragmentSpirV(vulkanBackend->device, vulkanBackend->physicalDevice);
    fragmentSpirV->LoadShaderFile(ps);
}

void VulkanPipeline::CreateRenderingPipeline()
{
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
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexSpirV->GetStageInfo(), fragmentSpirV->GetStageInfo()};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &vertexSpirV->GetDescriptorSetLayout();
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
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexSpirV->GetInputInfo();
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = vulkanBackend->renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
	auto result = vkCreateGraphicsPipelines(vulkanBackend->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to create graphics pipeline!";
        throw std::runtime_error("Failed to create graphics pipeline!");
    }

    LOG_INFO << "Created graphics pipeline.";

    createCommandBuffers();
}

VkPipeline& VulkanPipeline::GetPipeline()
{
    return graphicsPipeline;
}

void VulkanPipeline::createCommandBuffers()
{
    commandBuffers.resize(vulkanBackend->swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vulkanBackend->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanBackend->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        LOG_FATAL << "Failed to allocate command buffers!";
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    LOG_INFO << "Allocated command buffers.";

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to begin recording command buffer " << i;
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        LOG_INFO << "Recording command buffer " << i << " started.";

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkanBackend->renderPass;
        renderPassInfo.framebuffer = vulkanBackend->swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vulkanBackend->swapChainExtent;
        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                &vertexSpirV->descriptorSets[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indicesCount), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to record command buffer!";
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
}

void VulkanPipeline::LoadVertexData(void* vertexData, size_t vertexSize, size_t vertexCount)
{
    VkDeviceSize bufferSize = vertexSize * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    vulkanBackend->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void* data;
    vkMapMemory(vulkanBackend->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexData, (size_t) bufferSize);
    vkUnmapMemory(vulkanBackend->device, stagingBufferMemory);

    vulkanBackend->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    vulkanBackend->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(vulkanBackend->device, stagingBuffer, nullptr);
    vkFreeMemory(vulkanBackend->device, stagingBufferMemory, nullptr);
}

void VulkanPipeline::LoadIndexData(uint16_t* indexData, size_t indexCount)
{
    VkDeviceSize bufferSize = sizeof(uint16_t) * indexCount;
    indicesCount = static_cast<uint32_t>(indexCount);
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    vulkanBackend->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void* data;
    vkMapMemory(vulkanBackend->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indexData, (size_t) bufferSize);
    vkUnmapMemory(vulkanBackend->device, stagingBufferMemory);

    vulkanBackend->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    vulkanBackend->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(vulkanBackend->device, stagingBuffer, nullptr);
    vkFreeMemory(vulkanBackend->device, stagingBufferMemory, nullptr);
}

void VulkanPipeline::RecreatePipeline()
{
    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool,
                         static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(vulkanBackend->device, graphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vulkanBackend->device, pipelineLayout, nullptr);

    CreateRenderingPipeline();
}


