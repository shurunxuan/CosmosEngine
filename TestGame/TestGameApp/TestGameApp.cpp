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

    testCamera = new Camera();
    testCamera->Resize(float(screenWidth), float(screenHeight));
    testCamera->SetFov(glm::radians(45.0f));
    testCamera->SetNearPlane(0.1f);
    testCamera->SetFarPlane(1000.0f);

    return true;
}

void TestGameApp::Shutdown()
{
    delete testObject;

    delete testCamera;

    CEApp::Shutdown();
}

void TestGameApp::Update(float deltaTime, float totalTime)
{
    auto quaternion = testObject->transform->GetLocalRotation();
    quaternion = quaternion * glm::angleAxis(deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    testObject->transform->SetLocalRotation(quaternion);

//    float scale = (1.0f + sinf(totalTime * 2.0f) / 2.0f);
//    testObject->transform->SetLocalScale(scale, scale, scale);
//
//    glm::vec3 translation = {0.0f, sinf(totalTime * 3.2f) / 3.0f, 0.0f};
//    testObject->transform->SetLocalTranslation(translation);

    //testCamera->SetFov(glm::radians(52.5f + 7.5 * sinf(totalTime * 10.0f)));

    glm::vec3 translation = {0.0f, 2.0f, -5.0f};
    testCamera->transform->SetLocalTranslation(translation);

    glm::quat cameraRot0 = glm::angleAxis(glm::radians(20.0f * sinf(totalTime)), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat cameraRot1 = glm::angleAxis(glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    testCamera->transform->SetLocalRotation(cameraRot0 * cameraRot1);

}

