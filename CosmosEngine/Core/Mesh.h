//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <cstdint>
#include "../Export.h"

class ENGINE_API Mesh
{
public:
    Mesh();

    ~Mesh();

    void LoadVertexData(void* vertexData, size_t vertexSize, size_t vertexCount);

    void LoadIndexData(uint16_t* indexData, size_t indexCount);

    void* GetVertexBuffer();

    void* GetIndexBuffer();

    uint32_t GetIndexCount();

private:
    void* vertexBuffer;

    void* indexBuffer;

    uint32_t indexCount;
};

#endif //GAMEENGINE_MESH_H
