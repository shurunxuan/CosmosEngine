//
// Created by shuru on 9/8/2019.
//

#include "Material.h"
#include "../Rendering/RenderingBackend.h"

Material::Material()
{
    vertexShader = nullptr;
    pixelShader = nullptr;
}

Material::~Material()
{
    presentedRenderingBackend->DestroyVertexShader(&vertexShader);
    presentedRenderingBackend->DestroyPixelShader(&pixelShader);
}

void Material::LoadVertexShader(const boost::container::string& filename)
{
    vertexShader = presentedRenderingBackend->CreateVertexShader(filename);
}

void Material::LoadPixelShader(const boost::container::string& filename)
{
    pixelShader = presentedRenderingBackend->CreatePixelShader(filename);
}

ReflectionalShader* Material::GetVertexShader()
{
    return vertexShader;
}

ReflectionalShader* Material::GetPixelShader()
{
    return pixelShader;
}
