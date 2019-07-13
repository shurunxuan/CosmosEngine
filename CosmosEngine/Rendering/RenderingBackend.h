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

    bool Init(unsigned int screenWidth, unsigned int screenHeight);
    void Loop();
    void DeInit();

    bool ShouldTerminate();

    virtual bool StartUp() = 0;

    virtual void Update() = 0;

    virtual void Shutdown() = 0;

private:

    GLFWwindow* window;
};


#endif //COSMOSENGINE_RENDERINGBACKEND_H
