//
// Created by shuru on 9/7/2019.
//

#ifndef GAMEENGINE_RENDERINGPIPELINE_H
#define GAMEENGINE_RENDERINGPIPELINE_H

#include "../Export.h"
#include "ReflectionalShader.h"
#include "../Core/Mesh.h"
#include "../Core/Material.h"

#include <boost/container/string.hpp>

class ENGINE_API RenderingPipeline
{
public:
    RenderingPipeline(Mesh* mesh, Material* material);

    virtual ~RenderingPipeline();

    virtual void RecreatePipeline() = 0;

    virtual void CreateRenderingPipeline() = 0;

protected:
    Mesh* mesh;
    Material* material;

    boost::unordered_map<boost::container::string, ReflectionalShader*> varTable;
    boost::unordered_map<boost::container::string, ReflectionalShader*> imageTable;
    boost::unordered_map<boost::container::string, ReflectionalShader*> samplerTable;
};

#endif //GAMEENGINE_RENDERINGPIPELINE_H
