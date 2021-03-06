//
// Created by shuru on 7/13/2019.
//

#include "RenderingBackend.h"
#include "../Logging/Logging.h"
#include "../App/App.h"
#include "../Core/MeshRenderer.h"
#include "RenderingPipeline.h"

RenderingBackend* presentedRenderingBackend = nullptr;

RenderingBackend::RenderingBackend()
{
    window = nullptr;
    framebufferResized = false;
    exitSignal = false;
}

RenderingBackend::~RenderingBackend()
{

}

void RenderingBackend::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto backend = reinterpret_cast<RenderingBackend*>(glfwGetWindowUserPointer(window));
    backend->framebufferResized = true;
    backend->width = width;
    backend->height = height;
    App->CurrentActiveScene()->mainCamera->Resize(float(width), float(height));
}

bool RenderingBackend::StartUp(unsigned int screenWidth, unsigned int screenHeight)
{
    LOG_INFO << "Initializing Rendering Backend";

    width = screenWidth;
    height = screenHeight;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(screenWidth, screenHeight, "Cosmos Engine", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwGetFramebufferSize(window, &width, &height);

    Init();

    return true;
}

void RenderingBackend::Shutdown()
{
    DeInit();

    glfwDestroyWindow(window);
    glfwTerminate();

    LOG_INFO << "Rendering Backend Shutdown Completed.";
}

void RenderingBackend::Update(float deltaTime, float totalTime)
{
    glfwPollEvents();
    Render(deltaTime, totalTime);
}

bool RenderingBackend::ShouldTerminate()
{
    return exitSignal || glfwWindowShouldClose(window);
}

GLFWwindow* RenderingBackend::GetWindow()
{
    return window;
}

void RenderingBackend::ExitSignal()
{
    exitSignal = true;
}
