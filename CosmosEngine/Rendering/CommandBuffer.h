//
// Created by 西村智 on 10/2/19.
//

#ifndef GAMEENGINE_COMMANDBUFFER_H
#define GAMEENGINE_COMMANDBUFFER_H

#include "../Export.h"

class MeshRenderer;

class ENGINE_LOCAL CommandBuffer
{
public:

    explicit CommandBuffer(MeshRenderer* owner);

    virtual ~CommandBuffer();

    virtual void RecordCommandBuffer() = 0;

    void RecreateCommandBuffer();

protected:

    MeshRenderer* owner;

    virtual void CleanupCommandBuffer() = 0;
};

#endif //GAMEENGINE_COMMANDBUFFER_H
