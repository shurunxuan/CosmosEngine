//
// Created by 西村智 on 10/2/19.
//

#include "CommandBuffer.h"
#include "../Core/MeshRenderer.h"

CommandBuffer::CommandBuffer(MeshRenderer* owner)
{
    this->owner = owner;
}

CommandBuffer::~CommandBuffer()
= default;

void CommandBuffer::RecreateCommandBuffer()
{
    CleanupCommandBuffer();
    RecordCommandBuffer();
}
