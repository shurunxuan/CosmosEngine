//
// Created by shuru on 9/8/2019.
//

#include "Material.h"
#include "../Rendering/RenderingBackend.h"
#include "../Rendering/RenderingPipeline.h"

Material::Material()
{
    vertexShader = nullptr;
    pixelShader = nullptr;
    pipeline = nullptr;
}

Material::~Material()
{
    presentedRenderingBackend->DestroyVertexShader(&vertexShader);
    presentedRenderingBackend->DestroyPixelShader(&pixelShader);
}

void Material::LoadVertexShader(const boost::container::string& filename)
{
    vertexShader = presentedRenderingBackend->CreateVertexShader(filename);
    if (pixelShader != nullptr)
    {
        generateRenderingPipeline();
    }
}

void Material::LoadPixelShader(const boost::container::string& filename)
{
    pixelShader = presentedRenderingBackend->CreatePixelShader(filename);
    if (vertexShader != nullptr)
    {
        generateRenderingPipeline();
    }
}

ReflectionalShader* Material::GetVertexShader()
{
    return vertexShader;
}

ReflectionalShader* Material::GetPixelShader()
{
    return pixelShader;
}

RenderingPipeline* Material::GetPipeline()
{
    return pipeline;
}

void Material::generateRenderingPipeline()
{
    if (pipeline != nullptr)
    {
        presentedRenderingBackend->DestroyRenderingPipeline(&pipeline);
    }

    pipeline = presentedRenderingBackend->CreateRenderingPipeline(this);
}

void Material::SetTexture(const boost::container::string& textureName, const boost::shared_ptr<Texture>& texture)
{
    auto result = textures.insert(std::make_pair(textureName, texture));
    if (!result.second)
    {
        result.first->second = texture;
    }

    pipeline->SetSamplerTexture(textureName + "Sampler", textureName + "Texture", *texture);
}
