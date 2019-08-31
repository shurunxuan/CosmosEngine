#include <CosmosEngine/Logging/Logging.h>
#include "TestGameApp.h"
#include <boost/container/list.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

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

    testObject = CurrentActiveScene()->AddObject("TestObject");

    testCamera = CurrentActiveScene()->mainCamera;

    LOG_INFO << "Scene Structure:";
    boost::container::list<Object*> allObjects = CurrentActiveScene()->GetAllObjects();

    std::stack<Object*> objectStack;
    std::stack<int> objectLevel;

    for (Object* obj : boost::adaptors::reverse(allObjects))
    {
        if (obj->transform->GetParent() == nullptr)
        {
            objectStack.push(obj);
            objectLevel.push(0);
        }
    }

    while (!objectStack.empty())
    {
        // Get the object
        Object* currentObject = objectStack.top();
        int currentLevel = objectLevel.top();
        // Process DFS
        objectStack.pop();
        objectLevel.pop();
        boost::container::list<Transform*> children = currentObject->transform->GetChildren();
        for (Transform* child : boost::adaptors::reverse(children))
        {
            objectStack.push(child->object);
            objectLevel.push(currentLevel + 1);
        }

        // Output Info
        boost::container::string tabs = "\t";
        for (int i = 0; i < currentLevel; ++i)
            tabs += "\t";
        LOG_INFO << tabs << currentObject->name << "\t{" << currentObject->GetInstanceID() << "}";
    }

    return true;
}

void TestGameApp::Shutdown()
{
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

