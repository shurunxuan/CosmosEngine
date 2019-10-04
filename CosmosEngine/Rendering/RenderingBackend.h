//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_RENDERINGBACKEND_H
#define COSMOSENGINE_RENDERINGBACKEND_H

#include "../Export.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>

enum SamplerFilterMode
{
    FILTER_MODE_NEAREST = 0,
    FILTER_MODE_LINEAR = 1,
    FILTER_CUBIC = 2
};

enum SamplerAddressingMode
{
    SAMPLER_ADDRESS_MODE_REPEAT = 0,
    SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT = 1,
    SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE = 2,
    SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER = 3,
    SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE = 4
};

enum SamplerMipmapMode
{
    SAMPLER_MIPMAP_MODE_NEAREST = 0,
    SAMPLER_MIPMAP_MODE_LINEAR = 1
};

class MeshRenderer;
class Material;
class CommandBuffer;
class ReflectionalShader;
class RenderingPipeline;

class ENGINE_API RenderingBackend
{
public:
    RenderingBackend();

    virtual ~RenderingBackend();

    bool StartUp(unsigned int screenWidth, unsigned int screenHeight);

    void Update(float deltaTime, float totalTime);

    void Shutdown();

    bool ShouldTerminate();

    virtual bool Init() = 0;

    virtual void Render(float deltaTime, float totalTime) = 0;

    virtual void DeInit() = 0;

    virtual boost::container::vector<char> loadShader(const boost::container::string& filename) = 0;

    virtual RenderingPipeline* CreateRenderingPipeline(Material* material) = 0;

    virtual void DestroyRenderingPipeline(RenderingPipeline** pipeline) = 0;

    virtual ReflectionalShader* CreateVertexShader(const boost::container::string& filename) = 0;

    virtual void DestroyVertexShader(ReflectionalShader** shader) = 0;

    virtual ReflectionalShader* CreatePixelShader(const boost::container::string& filename) = 0;

    virtual void DestroyPixelShader(ReflectionalShader** shader) = 0;

    virtual void* CreateVertexBuffer(void* vertexData, size_t vertexSize, size_t vertexCount) = 0;

    virtual void DestroyVertexBuffer(void** vertexBuffer) = 0;

    virtual void* CreateIndexBuffer(uint16_t* indexData, size_t indexCount) = 0;

    virtual void DestroyIndexBuffer(void** indexBuffer) = 0;

    virtual void* CreateTexture(const boost::container::string& filename) = 0;

    virtual void DestroyTexture(void** texture) = 0;

    virtual void* CreateSampler(SamplerFilterMode filter, SamplerAddressingMode address, SamplerMipmapMode mipmap,
                                bool anisotropyEnable, float maxAnisotropy) = 0;

    virtual void DestroySampler(void** sampler) = 0;

    virtual CommandBuffer* CreateCommandBuffer(MeshRenderer* meshRenderer) = 0;

    virtual void DestroyCommandBuffer(CommandBuffer** commandBuffer) = 0;

    GLFWwindow* GetWindow();

protected:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window;

    int width;

    int height;

    bool framebufferResized = false;

};

ENGINE_EXTERNAL_VAR RenderingBackend* presentedRenderingBackend;

#endif //COSMOSENGINE_RENDERINGBACKEND_H
