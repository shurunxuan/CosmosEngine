//
// Created by shuru on 9/7/2019.
//

#include "RenderingPipeline.h"

RenderingPipeline::RenderingPipeline(Material* material)
{
    this->material = material;
}

RenderingPipeline::~RenderingPipeline()
= default;


bool RenderingPipeline::SetData(const boost::container::string& name, const void* data, unsigned int size)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetData(name, data, size);
}

bool RenderingPipeline::SetInt(const boost::container::string& name, int data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetInt(name, data);
}

bool RenderingPipeline::SetFloat(const boost::container::string& name, float data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat(name, data);
}

bool RenderingPipeline::SetFloat2(const boost::container::string& name, const float data[2])
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat2(name, data);
}

bool RenderingPipeline::SetFloat2(const boost::container::string& name, const glm::vec2& data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat2(name, data);
}

bool RenderingPipeline::SetFloat3(const boost::container::string& name, const float data[3])
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat3(name, data);
}

bool RenderingPipeline::SetFloat3(const boost::container::string& name, const glm::vec3& data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat3(name, data);
}

bool RenderingPipeline::SetFloat4(const boost::container::string& name, const float data[4])
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat4(name, data);
}


bool RenderingPipeline::SetFloat4(const boost::container::string& name, const glm::vec4& data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetFloat4(name, data);
}

bool RenderingPipeline::SetMatrix4x4(const boost::container::string& name, const float data[16])
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetMatrix4x4(name, data);
}

bool RenderingPipeline::SetMatrix4x4(const boost::container::string& name, const glm::mat4& data)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShader*>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return false;

    return result->second->SetMatrix4x4(name, data);
}