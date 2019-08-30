//
// Created by shuru on 7/15/2019.
//

#ifndef GAMEENGINE_REFLECTIONALSHADER_H
#define GAMEENGINE_REFLECTIONALSHADER_H

#include "../Export.h"

#include <Eigen/Dense>
#include <boost/unordered_map.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>

// --------------------------------------------------------
// Used by reflectional shaders to store information about
// specific variables in constant/uniform buffers
// --------------------------------------------------------
struct ReflectionalShaderVariable
{
    unsigned int ByteOffset;
    unsigned int Size;
    unsigned int BufferIndex;
};

// --------------------------------------------------------
// Contains information about a specific
// constant/uniform buffer in a shader, as well as
// the local data buffer for it
// --------------------------------------------------------
struct ReflectionalConstantBuffer
{
    boost::container::string Name;
    uint32_t Type; // To enum D3D_CBUFFER_TYPE, not relevant in SPIR-V
    unsigned int Size;
    unsigned int BindIndex;
    unsigned int SetIndex; // Not relevant in HLSL
    unsigned int LocationIndex; // Not relevant in HLSL
    void* ConstantBuffer; // void -> VkBuffer_T / ID3D11Buffer
    unsigned char* LocalDataBuffer;
    boost::container::vector<ReflectionalShaderVariable> Variables;
};

// --------------------------------------------------------
// TODO: Wait for confirmation
// Contains info about a single SRV in a shader
// --------------------------------------------------------
//struct ReflectionalSRV
//{
//    unsigned int Index;		// The raw index of the SRV
//    unsigned int BindIndex; // The register of the SRV
//};

// --------------------------------------------------------
// TODO: Wait for confirmation
// Contains info about a single Sampler in a shader
// --------------------------------------------------------
//struct ReflectionalSampler
//{
//    unsigned int Index;		// The raw index of the Sampler
//    unsigned int BindIndex; // The register of the Sampler
//};

// --------------------------------------------------------
// Base abstract class for simplifying shader handling
// --------------------------------------------------------
class ENGINE_API ReflectionalShader
{
public:
    ReflectionalShader();

    virtual ~ReflectionalShader() = 0;

    // Initialization method (since we can't invoke derived class
    // overrides in the base class constructor)
    virtual bool LoadShaderFile(const boost::container::string& shaderFile) = 0;

    bool IsShaderValid()
    { return shaderValid; }

    // Activating the shader and copying data
    void SetShader();

    virtual void CopyAllBufferData() = 0;

    virtual void CopyBufferData(unsigned int index) = 0;

    virtual void CopyBufferData(const boost::container::string& bufferName) = 0;

    // Sets arbitrary shader data
    bool SetData(const boost::container::string& name, const void* data, unsigned int size);

    bool SetInt(const boost::container::string& name, int data);

    bool SetFloat(const boost::container::string& name, float data);

    bool SetFloat2(const boost::container::string& name, const float data[2]);

    bool SetFloat2(const boost::container::string& name, const Eigen::Vector2f& data);

    bool SetFloat3(const boost::container::string& name, const float data[3]);

    bool SetFloat3(const boost::container::string& name, const Eigen::Vector3f& data);

    bool SetFloat4(const boost::container::string& name, const float data[4]);

    bool SetFloat4(const boost::container::string& name, const Eigen::Vector4f& data);

    bool SetMatrix4x4(const boost::container::string& name, const float data[16]);

    bool SetMatrix4x4(const boost::container::string& name, const Eigen::Matrix4f& data);

    // Setting shader resources
    // TODO: Declare this after class Texture is well defined
    //virtual bool SetImage(const boost::container::string& name, ID3D11ShaderResourceView* srv) = 0;
    //virtual bool SetSampler(const boost::container::string& name, ID3D11SamplerState* samplerState) = 0;

    // Getting data about variables and resources
    const ReflectionalShaderVariable* GetVariableInfo(const boost::container::string& name);

    //virtual const ReflectionalSRV* GetShaderResourceViewInfo(const boost::container::string& name) = 0;
    //virtual const ReflectionalSRV* GetShaderResourceViewInfo(unsigned int index) = 0;
    //size_t GetShaderResourceViewCount() { return textureTable.size(); }

    //virtual const ReflectionalSampler* GetSamplerInfo(const boost::container::string& name) = 0;
    //virtual const ReflectionalSampler* GetSamplerInfo(unsigned int index) = 0;
    //size_t GetSamplerCount() { return samplerTable.size(); }

    // Get data about constant buffers
    unsigned int GetBufferCount();

    unsigned int GetBufferSize(unsigned int index);

    const ReflectionalConstantBuffer* GetBufferInfo(const boost::container::string& name);

    const ReflectionalConstantBuffer* GetBufferInfo(unsigned int index);

protected:

    bool shaderValid;
    //void* shaderBlob; // void -> VkShaderModule_T / ID3D11Blob
//    ID3D11Device* device;
//    ID3D11DeviceContext* deviceContext;

    // Resource counts
    unsigned int constantBufferCount;

    // Maps for variables and buffers
    ReflectionalConstantBuffer* constantBuffers; // For index-based lookup
//    boost::container::vector<ReflectionalSRV*>		shaderResourceViews;
//    boost::container::vector<ReflectionalSampler*>	samplerStates;
    boost::unordered_map<boost::container::string, ReflectionalConstantBuffer*> cbTable;
    boost::unordered_map<boost::container::string, ReflectionalShaderVariable> varTable;
//    boost::unordered_map<boost::container::string, ReflectionalSRV*> textureTable;
//    boost::unordered_map<boost::container::string, ReflectionalSampler*> samplerTable;

    // Pure virtual functions for dealing with shader types
    virtual void SetShaderAndCBs() = 0;

    virtual void CleanUp();

    virtual void ReleaseConstantBuffer(size_t index) = 0;

    // Helpers for finding data by name
    ReflectionalShaderVariable* FindVariable(const boost::container::string& name, int size);

    ReflectionalConstantBuffer* FindConstantBuffer(const boost::container::string& name);
};

#endif //GAMEENGINE_REFLECTIONALSHADER_H
