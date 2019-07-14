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

bool RenderingBackend::StartUp(unsigned int screenWidth, unsigned int screenHeight)
{
    LOG_INFO << "Initializing Rendering Backend";

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(screenWidth, screenHeight, "Cosmos Engine", nullptr, nullptr);

    Init();

    return true;
}

void RenderingBackend::Shutdown()
{
    DeInit();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void RenderingBackend::Update()
{
    glfwPollEvents();
    Render();
}

bool RenderingBackend::ShouldTerminate()
{
    return glfwWindowShouldClose(window);
}
