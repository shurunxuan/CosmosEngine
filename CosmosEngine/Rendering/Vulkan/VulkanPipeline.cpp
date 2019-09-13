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
    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(vulkanBackend->device, graphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vulkanBackend->device, pipelineLayout, nullptr);
}

void VulkanPipeline::CreateRenderingPipeline()
{
    VertexSpirV* vertexSpirV = dynamic_cast<VertexSpirV*>(material->GetVertexShader());
    FragmentSpirV* fragmentSpirV = dynamic_cast<FragmentSpirV*>(material->GetPixelShader());

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

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexSpirV->GetStageInfo(), fragmentSpirV->GetStageInfo()};
    boost::container::vector<VkDescriptorSetLayout> setLayouts;

    if (vertexSpirV->hasDescriptors)
        setLayouts.push_back(vertexSpirV->GetDescriptorSetLayout());

    if (fragmentSpirV->hasDescriptors)
        setLayouts.push_back(fragmentSpirV->GetDescriptorSetLayout());

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
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
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
	auto result = vkCreateGraphicsPipelines(vulkanBackend->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
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

        LOG_INFO << "Recording command buffer " << i << " started.";

//        VkRenderPassBeginInfo renderPassInfo = {};
//        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//        renderPassInfo.renderPass = vulkanBackend->renderPass;
//        renderPassInfo.framebuffer = vulkanBackend->swapChainFramebuffers[i];
//        renderPassInfo.renderArea.offset = {0, 0};
//        renderPassInfo.renderArea.extent = vulkanBackend->swapChainExtent;
//        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
//        renderPassInfo.clearValueCount = 1;
//        renderPassInfo.pClearValues = &clearColor;
//
//        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {vBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], iBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                &vertexSpirV->descriptorSets[i], 0, nullptr);

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

void VulkanPipeline::RecreatePipeline()
{
    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool,
                         static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(vulkanBackend->device, graphicsPipeline, nullptr);

    vkDestroyPipelineLayout(vulkanBackend->device, pipelineLayout, nullptr);

    CreateRenderingPipeline();
}


