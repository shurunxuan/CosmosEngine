//
// Created by shuru on 9/8/2019.
//

#include "Mesh.h"
#include "../Rendering/RenderingBackend.h"

Mesh::Mesh()
{
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    indexCount = 0;
}

Mesh::~Mesh()
{
    presentedRenderingBackend->DestroyVertexBuffer(&vertexBuffer);
    presentedRenderingBackend->DestroyIndexBuffer(&indexBuffer);
}

void Mesh::LoadVertexData(void* vertexData, size_t vertexSize, size_t vertexCount)
{
    vertexBuffer = presentedRenderingBackend->CreateVertexBuffer(vertexData, vertexSize, vertexCount);
}

void Mesh::LoadIndexData(uint16_t* indexData, size_t count)
{
    indexBuffer = presentedRenderingBackend->CreateIndexBuffer(indexData, count);
    indexCount = static_cast<uint32_t>(count);
}

void* Mesh::GetVertexBuffer()
{
    return vertexBuffer;
}

void* Mesh::GetIndexBuffer()
{
    return indexBuffer;
}

uint32_t Mesh::GetIndexCount()
{
    return indexCount;
}
