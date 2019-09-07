//
// Created by shuru on 9/7/2019.
//

#ifndef GAMEENGINE_RENDERINGPIPELINE_H
#define GAMEENGINE_RENDERINGPIPELINE_H

#include "../Export.h"
#include "ReflectionalShader.h"

#include <boost/container/string.hpp>

class ENGINE_API RenderingPipeline
{
public:
    RenderingPipeline();

    virtual ~RenderingPipeline();

    virtual void SetVertexShader(const boost::container::string& vs) = 0;

    virtual void SetPixelShader(const boost::container::string& ps) = 0;



    virtual void CreateRenderingPipeline() = 0;

    virtual void LoadVertexData(void* data, size_t vertexSize, size_t vertexCount) = 0;

    virtual void LoadIndexData(uint16_t* indexData, size_t indexCount) = 0;

protected:
};

#endif //GAMEENGINE_RENDERINGPIPELINE_H
