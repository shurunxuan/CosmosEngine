#include <CosmosEngine/Logging/Logging.h>
#include "TestGameApp.h"

TestGameApp::TestGameApp()
    : CEApp()
{

}

TestGameApp::~TestGameApp()
{

}

bool TestGameApp::StartUp(unsigned int screenWidth, unsigned int screenHeight)
{
    CEApp::StartUp(screenWidth, screenHeight);
    return true;
}

void TestGameApp::Shutdown()
{
    CEApp::Shutdown();
}

