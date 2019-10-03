//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_MATERIAL_H
#define GAMEENGINE_MATERIAL_H

#include "../Export.h"
#include "../Rendering/ReflectionalShader.h"
#include "Texture.h"

#include <boost/container/string.hpp>
#include <boost/container/map.hpp>

class RenderingPipeline;

class ENGINE_API Material
{
public:
    Material();

    ~Material();

    void LoadVertexShader(const boost::container::string& filename);

    void LoadPixelShader(const boost::container::string& filename);

    /**
     * @brief Get the Material Struct
     *
     * This must be implemented by a material
     *
     * @param[out] mtlPtr The pointer points to the material struct
     * @return size_t The size of the struct
     */
    //virtual size_t GetMaterialStruct(void** mtlPtr) = 0;

    ReflectionalShader* GetVertexShader();

    ReflectionalShader* GetPixelShader();

    RenderingPipeline* GetPipeline();

    void SetTexture(const boost::container::string& textureName, const boost::shared_ptr<Texture>& texture);

protected:
    /**
     * @brief Set other data needed by the shaders
     *
     * This must be implemented even if there's no other things to set
     *
     */
    //virtual void SetShaderData() = 0;

    void generateRenderingPipeline();

    ReflectionalShader* vertexShader;

    ReflectionalShader* pixelShader;

    RenderingPipeline* pipeline;

    boost::container::map<boost::container::string, boost::shared_ptr<Texture>> textures;
};

#endif //GAMEENGINE_MATERIAL_H
