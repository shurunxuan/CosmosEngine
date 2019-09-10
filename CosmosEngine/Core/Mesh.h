//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include "../Export.h"

struct ENGINE_API DefaultVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
};

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
