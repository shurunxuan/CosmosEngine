//
// Created by 西村智 on 2019-05-15.
//

#include "App.h"

#include "../Rendering/Vulkan/VulkanBackend.h"
#include "../Logging/Logging.h"

#include <boost/version.hpp>
#include <boost/config.hpp>

CEApp* App = nullptr;

int CosmosEngine(int argc, char** argv)
{
    App->StartUp(1366, 768);
    App->Loop();
    App->Shutdown();

    return 0;
}

CEApp::CEApp()
{
    App = this;
    InitLogger();
    renderingBackend = nullptr;
}

CEApp::~CEApp()
{
    delete renderingBackend;
    StopLogger();
}

bool CEApp::StartUp(unsigned int screenWidth, unsigned int screenHeight)
{
    LOG_INFO << "Starting Up CosmosEngine Application";

    LOG_INFO << "Platform: " << BOOST_PLATFORM;
    LOG_INFO << "Compiler: " << BOOST_COMPILER;

    LOG_INFO << "Boost Version: "
             << BOOST_VERSION / 100000 << "."
             << BOOST_VERSION / 100 % 1000 << "."
             << BOOST_VERSION % 100;

    // TODO: Choose Rendering Backend
    renderingBackend = new VulkanBackend();

    renderingBackend->StartUp(screenWidth, screenHeight);


    startTime = boost::chrono::high_resolution_clock::now();
    currentTime = startTime;
    lastTime = startTime;
    deltaTime = currentTime - lastTime;
    totalTime = currentTime - startTime;
    return true;
}

void CEApp::Loop()
{
    while (!renderingBackend->ShouldTerminate())
    {
        lastTime = currentTime;

        // TODO: This is actually Scene::Update, don't forget to remove it
        App->Update(deltaTime.count(), totalTime.count());
        renderingBackend->Update(deltaTime.count(), totalTime.count());

        currentTime = boost::chrono::high_resolution_clock::now();
        deltaTime = currentTime - lastTime;
        totalTime = currentTime - startTime;
    }
}

void CEApp::Shutdown()
{
    renderingBackend->Shutdown();
}


