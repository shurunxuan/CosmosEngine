//
// Created by shuru on 7/13/2019.
//

#ifndef COSMOSENGINE_RENDERINGBACKEND_H
#define COSMOSENGINE_RENDERINGBACKEND_H

#include "../Export.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

class ENGINE_API RenderingBackend
{
public:
    RenderingBackend();

    virtual ~RenderingBackend();

    bool StartUp(unsigned int screenWidth, unsigned int screenHeight);

    void Update();

    void Shutdown();

    bool ShouldTerminate();

    virtual bool Init() = 0;

    virtual void Render() = 0;

    virtual void DeInit() = 0;

protected:

    GLFWwindow* window;

    unsigned int width;

    unsigned int height;
};


#endif //COSMOSENGINE_RENDERINGBACKEND_H
