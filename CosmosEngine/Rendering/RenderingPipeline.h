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

class Texture;

class ENGINE_API RenderingPipeline
{
public:
    RenderingPipeline(Mesh* mesh, Material* material);

    virtual ~RenderingPipeline();

    virtual void RecreatePipeline() = 0;

    virtual void CreateRenderingPipeline() = 0;

    bool SetData(const boost::container::string& name, const void* data, unsigned int size);

    bool SetInt(const boost::container::string& name, int data);

    bool SetFloat(const boost::container::string& name, float data);

    bool SetFloat2(const boost::container::string& name, const float data[2]);

    bool SetFloat2(const boost::container::string& name, const glm::vec2& data);

    bool SetFloat3(const boost::container::string& name, const float data[3]);

    bool SetFloat3(const boost::container::string& name, const glm::vec3& data);

    bool SetFloat4(const boost::container::string& name, const float data[4]);

    bool SetFloat4(const boost::container::string& name, const glm::vec4& data);

    bool SetMatrix4x4(const boost::container::string& name, const float data[16]);

    bool SetMatrix4x4(const boost::container::string& name, const glm::mat4& data);

    virtual bool SetTexture(const boost::container::string& name, const Texture& texture) = 0;

    virtual bool SetSampler(const boost::container::string& name, const Texture& texture) = 0;

    virtual bool
    SetSamplerTexture(const boost::container::string& samplerName, const boost::container::string& textureName,
                      const Texture& texture) = 0;
protected:
    Mesh* mesh;
    Material* material;

    boost::unordered_map<boost::container::string, ReflectionalShader*> varTable;
    boost::unordered_map<boost::container::string, ReflectionalShader*> imageTable;
    boost::unordered_map<boost::container::string, ReflectionalShader*> samplerTable;
};

#endif //GAMEENGINE_RENDERINGPIPELINE_H
