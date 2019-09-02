//
// Created by shuru on 7/15/2019.
//

#include "ReflectionalShader.h"

ReflectionalShader::ReflectionalShader()
{
    shaderValid = false;
    constantBufferCount = 0;
    constantBuffers = nullptr;
}

ReflectionalShader::~ReflectionalShader()
{

}

bool ReflectionalShader::SetData(const boost::container::string& name, const void* data, unsigned int size)
{
    // Look for the variable and verify
    ReflectionalShaderVariable* var = FindVariable(name, size);
    if (var == 0)
        return false;

    // Set the data in the local data buffer
    memcpy(
            constantBuffers[var->BufferIndex].LocalDataBuffer + var->ByteOffset,
            data,
            size);

    // Success
    return true;
}

bool ReflectionalShader::SetInt(const boost::container::string& name, int data)
{
    return SetData(name, (void*) (&data), sizeof(int));
}

bool ReflectionalShader::SetFloat(const boost::container::string& name, float data)
{
    return SetData(name, (void*) (&data), sizeof(float));
}

bool ReflectionalShader::SetFloat2(const boost::container::string& name, const float* data)
{
    return SetData(name, (void*) (data), sizeof(float) * 2);
}

bool ReflectionalShader::SetFloat2(const boost::container::string& name, const glm::vec2& data)
{
    return SetData(name, (void*) (&data), sizeof(float) * 2);
}

bool ReflectionalShader::SetFloat3(const boost::container::string& name, const float* data)
{
    return SetData(name, (void*) (data), sizeof(float) * 3);
}

bool ReflectionalShader::SetFloat3(const boost::container::string& name, const glm::vec3& data)
{
    return SetData(name, (void*) (&data), sizeof(float) * 3);
}

bool ReflectionalShader::SetFloat4(const boost::container::string& name, const float* data)
{
    return SetData(name, (void*) (data), sizeof(float) * 4);
}

bool ReflectionalShader::SetFloat4(const boost::container::string& name, const glm::vec4& data)
{
    return SetData(name, (void*) (&data), sizeof(float) * 4);
}

bool ReflectionalShader::SetMatrix4x4(const boost::container::string& name, const float* data)
{
    return SetData(name, (void*) (data), sizeof(float) * 16);
}

bool ReflectionalShader::SetMatrix4x4(const boost::container::string& name, const glm::mat4& data)
{
    return SetData(name, (void*) (&data), sizeof(float) * 16);
}

const ReflectionalShaderVariable* ReflectionalShader::GetVariableInfo(const boost::container::string& name)
{
    return FindVariable(name, -1);
}

ReflectionalShaderVariable* ReflectionalShader::FindVariable(const boost::container::string& name, int size)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalShaderVariable>::iterator result =
            varTable.find(name);

    // Did we find the key?
    if (result == varTable.end())
        return nullptr;

    // Grab the result from the iterator
    ReflectionalShaderVariable* var = &(result->second);

    // Is the data size correct ?
    if (size > 0 && var->Size != size)
        return nullptr;

    // Success
    return var;
}

ReflectionalConstantBuffer* ReflectionalShader::FindConstantBuffer(const boost::container::string& name)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalConstantBuffer*>::iterator result =
            cbTable.find(name);

    // Did we find the key?
    if (result == cbTable.end())
        return nullptr;

    // Success
    return result->second;
}

void ReflectionalShader::SetShader()
{
    // Ensure the shader is valid
    if (!shaderValid) return;

    // Set the shader and any relevant constant buffers, which
    // is an overloaded method in a subclass
    SetShaderAndCBs();
}

unsigned int ReflectionalShader::GetBufferCount()
{
    return constantBufferCount;
}

unsigned int ReflectionalShader::GetBufferSize(unsigned int index)
{
    // Valid index?
    if (index >= constantBufferCount)
        return -1;

    // Grab the size
    return constantBuffers[index].Size;
}

const ReflectionalConstantBuffer* ReflectionalShader::GetBufferInfo(const boost::container::string& name)
{
    return FindConstantBuffer(name);
}

const ReflectionalConstantBuffer* ReflectionalShader::GetBufferInfo(unsigned int index)
{
    // Check for valid index
    if (index >= constantBufferCount) return nullptr;

    // Return the specific buffer
    return &constantBuffers[index];
}

void ReflectionalShader::CleanUp()
{
    // Handle constant buffers and local data buffers
    for (unsigned int i = 0; i < constantBufferCount; i++)
    {
        ReleaseConstantBuffer(i);
        delete[] constantBuffers[i].LocalDataBuffer;
    }

    if (constantBuffers)
    {
        delete[] constantBuffers;
        constantBufferCount = 0;
    }

//    for (unsigned int i = 0; i < shaderResourceViews.size(); i++)
//        delete shaderResourceViews[i];
//
//    for (unsigned int i = 0; i < samplerStates.size(); i++)
//        delete samplerStates[i];

    // Clean up tables
    varTable.clear();
    cbTable.clear();
//    samplerTable.clear();
//    textureTable.clear();
}
