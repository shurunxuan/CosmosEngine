//
// Created by shuru on 9/30/2019.
//

#include "Texture.h"

Texture::Texture()
{
    textureData = nullptr;
    sampler = nullptr;
}

Texture::~Texture()
{
    if (textureData != nullptr)
        presentedRenderingBackend->DestroyTexture(&textureData);

    if (sampler != nullptr)
        presentedRenderingBackend->DestroySampler(&sampler);
}

void Texture::LoadTexture(const boost::container::string& filename)
{
    if (textureData != nullptr)
        presentedRenderingBackend->DestroyTexture(&textureData);

    textureData = presentedRenderingBackend->CreateTexture(filename);
}

void Texture::SetSamplerMode(SamplerFilterMode filter, SamplerAddressingMode address, SamplerMipmapMode mipmap,
                             bool anisotropyEnable, float maxAnisotropy)
{
    if (sampler != nullptr)
        presentedRenderingBackend->DestroySampler(&sampler);

    sampler = presentedRenderingBackend->CreateSampler(filter, address, mipmap, anisotropyEnable, maxAnisotropy);
}

void* Texture::GetTextureData() const
{
    return textureData;
}

void* Texture::GetSampler() const
{
    return sampler;
}
