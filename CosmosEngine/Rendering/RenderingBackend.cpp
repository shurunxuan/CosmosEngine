//
// Created by shuru on 7/13/2019.
//

#include "RenderingBackend.h"
#include "../Logging/Logging.h"

RenderingBackend::RenderingBackend()
{
    window = nullptr;
}

RenderingBackend::~RenderingBackend()
{

}

bool RenderingBackend::Init(unsigned int screenWidth, unsigned int screenHeight)
{
    LOG_INFO << "Initializing Rendering Backend";

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(screenWidth, screenHeight, "Cosmos Engine", nullptr, nullptr);

    StartUp();

    return true;
}

void RenderingBackend::DeInit()
{
    Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void RenderingBackend::Loop()
{
    glfwPollEvents();
    Update();
}

bool RenderingBackend::ShouldTerminate()
{
    return glfwWindowShouldClose(window);
}
