//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_RENDERINGBACKEND_H
#define COSMOSENGINE_RENDERINGBACKEND_H

#include "../Export.h"
#include "RenderingPipeline.h"
#include "../Core/MeshRenderer.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>

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

    virtual RenderingPipeline* CreateRenderingPipeline(Mesh* mesh, Material* material) = 0;

    virtual void DestroyRenderingPipeline(RenderingPipeline** pipeline) = 0;

    virtual ReflectionalShader* CreateVertexShader(const boost::container::string& filename) = 0;

    virtual void DestroyVertexShader(ReflectionalShader** shader) = 0;

    virtual ReflectionalShader* CreatePixelShader(const boost::container::string& filename) = 0;

    virtual void DestroyPixelShader(ReflectionalShader** shader) = 0;

    virtual void* CreateVertexBuffer(void* vertexData, size_t vertexSize, size_t vertexCount) = 0;

    virtual void DestroyVertexBuffer(void** vertexBuffer) = 0;

    virtual void* CreateIndexBuffer(uint16_t* indexData, size_t indexCount) = 0;

    virtual void DestroyIndexBuffer(void** indexBuffer) = 0;

protected:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window;

    int width;

    int height;

    bool framebufferResized = false;

};

ENGINE_EXTERNAL_VAR RenderingBackend* presentedRenderingBackend;

#endif //COSMOSENGINE_RENDERINGBACKEND_H
