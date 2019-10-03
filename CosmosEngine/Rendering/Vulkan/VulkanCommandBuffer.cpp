//
// Created by 西村智 on 10/2/19.
//

#include "VulkanCommandBuffer.h"
#include "../../Core/MeshRenderer.h"
#include "VulkanBackend.h"
#include "../../Logging/Logging.h"

VulkanCommandBuffer::VulkanCommandBuffer(MeshRenderer* owner) : CommandBuffer(owner)
{

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    CleanupCommandBuffer();
}

void VulkanCommandBuffer::RecordCommandBuffer()
{
    VulkanPipeline* materialPipeline = reinterpret_cast<VulkanPipeline*>(owner->GetMaterial()->GetPipeline());
    VkPipeline pipeline = materialPipeline->GetPipeline();
    VkPipelineLayout pipelineLayout = materialPipeline->GetPipelineLayout();

    boost::container::vector<boost::container::vector<VkDescriptorSet>> transposedDescriptorSets;
    transposedDescriptorSets.resize(vulkanBackend->GetSwapChainImageCount());
    transposedDescriptorSets[0].reserve(materialPipeline->descriptorSets.size());
    transposedDescriptorSets[1].reserve(materialPipeline->descriptorSets.size());
    transposedDescriptorSets[2].reserve(materialPipeline->descriptorSets.size());
    for (int i = 0; i < materialPipeline->descriptorSets.size(); ++i)
    {
        transposedDescriptorSets[0].push_back(materialPipeline->descriptorSets[i][0]);
        transposedDescriptorSets[1].push_back(materialPipeline->descriptorSets[i][1]);
        transposedDescriptorSets[2].push_back(materialPipeline->descriptorSets[i][2]);
    }

    // Create and Record Command Buffers
    VulkanBufferWithMemory* vBuffer = reinterpret_cast<VulkanBufferWithMemory*>(owner->GetMesh()->GetVertexBuffer());
    VulkanBufferWithMemory* iBuffer = reinterpret_cast<VulkanBufferWithMemory*>(owner->GetMesh()->GetIndexBuffer());

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

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        VkBuffer vertexBuffers[] = {vBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[i], iBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(transposedDescriptorSets[i].size()),
                                transposedDescriptorSets[i].data(), 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], owner->GetMesh()->GetIndexCount(), 1, 0, 0, 0);

        //        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        {
            LOG_FATAL << "Failed to record command buffer!";
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
}

void VulkanCommandBuffer::CleanupCommandBuffer()
{
    vkFreeCommandBuffers(vulkanBackend->device, vulkanBackend->commandPool,
                         static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}
