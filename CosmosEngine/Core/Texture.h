//
// Created by shuru on 9/30/2019.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include "../Export.h"
#include "../Rendering/RenderingBackend.h"

class ENGINE_API Texture
{
public:
    Texture();

    ~Texture();

    void LoadTexture(const boost::container::string& filename);

    void SetSamplerMode(SamplerFilterMode filter, SamplerAddressingMode address, SamplerMipmapMode mipmap,
                        bool anisotropyEnable, float maxAnisotropy);

    void* GetTextureData() const;

    void* GetSampler() const;

private:
    void* textureData;
    void* sampler;
};


#endif //GAMEENGINE_TEXTURE_H
