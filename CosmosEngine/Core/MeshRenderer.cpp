//
// Created by shuru on 9/8/2019.
//

#include "MeshRenderer.h"
#include "../Rendering/RenderingBackend.h"
#include "../Rendering/RenderingPipeline.h"

MeshRenderer::MeshRenderer(Object* owner) : Component(owner)
{
    mesh = nullptr;
    material = nullptr;
    commandBuffer = nullptr;
}

MeshRenderer::~MeshRenderer()
{
    if (commandBuffer != nullptr)
    {
        presentedRenderingBackend->DestroyCommandBuffer(&commandBuffer);
    }
}

void MeshRenderer::Start()
{

}

void MeshRenderer::Update(float deltaTime, float totalTime)
{

}

void MeshRenderer::SetMesh(const boost::shared_ptr<Mesh>& m)
{
    mesh = m;
    if (material != nullptr)
    {
        createCommandBuffer();
    }
}

void MeshRenderer::SetMaterial(const boost::shared_ptr<Material>& m)
{
    material = m;
    if (mesh != nullptr)
    {
        createCommandBuffer();
    }
}

boost::shared_ptr<Material>& MeshRenderer::GetMaterial()
{
    return material;
}

boost::shared_ptr<Mesh>& MeshRenderer::GetMesh()
{
    return mesh;
}

void MeshRenderer::RecreateRenderingResources()
{
    material->GetPipeline()->RecreatePipeline();
    commandBuffer->RecreateCommandBuffer();
}

void MeshRenderer::createCommandBuffer()
{
    commandBuffer = presentedRenderingBackend->CreateCommandBuffer(this);
}

CommandBuffer* MeshRenderer::GetCommandBuffer()
{
    return commandBuffer;
}

