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

    testObject = new Object();

    return true;
}

void TestGameApp::Shutdown()
{
    delete testObject;

    CEApp::Shutdown();
}

void TestGameApp::Update(float deltaTime, float totalTime)
{
    auto quaternion = testObject->transform->GetLocalRotation();
    quaternion = quaternion * glm::angleAxis(deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    testObject->transform->SetLocalRotation(quaternion);

    float scale = (1.0f + sinf(totalTime * 2.0f) / 2.0f);
    testObject->transform->SetLocalScale(scale, scale, scale);

    glm::vec3 translation = {0.0f, sinf(totalTime * 3.2f) / 3.0f, 0.0f};
    testObject->transform->SetLocalTranslation(translation);
}

