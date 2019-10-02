//
// Created by 西村智 on 10/2/19.
//

#ifndef GAMEENGINE_VULKANCOMMANDBUFFER_H
#define GAMEENGINE_VULKANCOMMANDBUFFER_H

#include <boost/container/vector.hpp>
#include <vulkan/vulkan.h>

#include "../CommandBuffer.h"

class ENGINE_LOCAL VulkanCommandBuffer : public CommandBuffer
{
public:
    friend class VulkanBackend;

    explicit VulkanCommandBuffer(MeshRenderer* owner);

    ~VulkanCommandBuffer() final;

    void RecordCommandBuffer() final;

private:

    void CleanupCommandBuffer() final;

    boost::container::vector<VkCommandBuffer> commandBuffers;
};

#endif //GAMEENGINE_VULKANCOMMANDBUFFER_H
