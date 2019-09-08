//
// Created by shuru on 9/8/2019.
//

#include "MeshRenderer.h"
#include "../Rendering/RenderingBackend.h"

MeshRenderer::MeshRenderer(Object* owner) : Component(owner)
{
    mesh = nullptr;
    material = nullptr;
    pipeline = nullptr;
}

MeshRenderer::~MeshRenderer()
{

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
        generateRenderingPipeline();
    }
}

void MeshRenderer::SetMaterial(const boost::shared_ptr<Material>& m)
{
    material = m;
    if (mesh != nullptr)
    {
        generateRenderingPipeline();
    }
}

void MeshRenderer::generateRenderingPipeline()
{
    if (pipeline != nullptr)
    {
        presentedRenderingBackend->DestroyRenderingPipeline(&pipeline);
    }

    pipeline = presentedRenderingBackend->CreateRenderingPipeline(mesh.get(), material.get());
}

boost::shared_ptr<Material>& MeshRenderer::GetMaterial()
{
    return material;
}

boost::shared_ptr<Mesh>& MeshRenderer::GetMesh()
{
    return mesh;
}

RenderingPipeline* MeshRenderer::GetPipeline()
{
    return pipeline;
}
