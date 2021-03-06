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
    delete[] constantBuffers;
}

size_t ReflectionalShader::GetBufferCount()
{
    return constantBufferCount;
}

bool ReflectionalShader::SetData(const boost::container::string& name, const void* data, unsigned int size)
{
    // Look for the variable and verify
    ReflectionalShaderVariable* var = FindVariable(name, size);
    if (var == nullptr)
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
    if (transposeMatrix)
    {
        glm::mat4 transposed(data[0], data[4], data[8], data[12],
                             data[1], data[5], data[9], data[13],
                             data[2], data[6], data[10], data[14],
                             data[3], data[7], data[11], data[15]);
        return SetData(name, (void*) (&transposed), sizeof(float) * 16);
    }
    return SetData(name, (void*) (data), sizeof(float) * 16);
}

bool ReflectionalShader::SetMatrix4x4(const boost::container::string& name, const glm::mat4& data)
{
    if (transposeMatrix)
    {
        glm::mat4 transposed = glm::transpose(data);
        return SetData(name, (void*) (&transposed), sizeof(float) * 16);
    }
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

ReflectionalConstantBuffer* ReflectionalShader::FindConstantBuffer(const boost::container::string& name, size_t* index)
{
    // Look for the key
    boost::unordered_map<boost::container::string, ReflectionalConstantBuffer*>::iterator result =
            cbTable.find(name);

    if (index != nullptr)
        *index = std::distance(cbTable.begin(), result);

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
    for (size_t i = 0; i < constantBufferCount; i++)
    {
        ReleaseConstantBuffer(i);
        delete[] constantBuffers[i].LocalDataBuffer;
    }

    if (constantBuffers)
    {
        delete[] constantBuffers;
        constantBufferCount = 0;
    }

    for (auto& textureView : textureViews)
        delete textureView;

    for (auto& samplerState : samplerStates)
        delete samplerState;

    // Clean up tables
    varTable.clear();
    cbTable.clear();
    samplerTable.clear();
    textureTable.clear();
}
