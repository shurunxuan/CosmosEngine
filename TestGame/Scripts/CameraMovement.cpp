//
// Created by shuru on 9/8/2019.
//

#include "CameraMovement.h"
#include <CosmosEngine/Core/Object.h>
#include <CosmosEngine/Input/InputBackend.h>
#include <CosmosEngine/App/App.h>

CameraMovement::CameraMovement(Object* owner) : Component(owner)
{

}

CameraMovement::~CameraMovement()
{

}

struct TestObject2
{
    float value = 11.2f;

    SERIALIZABLE_CLASS;

    SERIALIZER
    {
        ADD_FIELD(value);
    };
};

struct TestObject
{
    float value = 12.3f;
    TestObject2 object2;


    SERIALIZABLE_CLASS;

    SERIALIZER
    {
        ADD_FIELD(value);
        ADD_FIELD(object2);
    };
};

SERIALIZATION_VERSION(TestObject, 1);
SERIALIZATION_VERSION(TestObject2, 1);

void CameraMovement::Start()
{
    std::ofstream ofs("test.txt");
    SerializerO ar(ofs);
    TestObject object;
    ar(SERIALIZED_FIELD(object));
}

void CameraMovement::Update(float deltaTime, float totalTime)
{
    if (presentedInputBackend->GetButtonDown("Exit"))
    {
        App->Exit();
    }

    auto translation = object->transform->GetLocalTranslation();

    float horizontal = presentedInputBackend->GetAxis("Horizontal");
    float vertical = presentedInputBackend->GetAxis("Vertical");

    translation +=
            (horizontal * deltaTime * object->transform->Right() +
             vertical * deltaTime * object->transform->Forward()) * 2.0f;

    object->transform->SetLocalTranslation(translation);

    glm::quat rotation = object->transform->GetLocalRotation();

    float cameraHorizontal = presentedInputBackend->GetAxis("CameraHorizontal");
    float cameraVertical = presentedInputBackend->GetAxis("CameraVertical");

    //glm::quat cameraRot0 = glm::angleAxis(glm::radians(20.0f * sinf(totalTime)), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat cameraRot0 = glm::angleAxis(cameraVertical * deltaTime, object->transform->Right());
    glm::quat cameraRot1 = glm::angleAxis(cameraHorizontal * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
    object->transform->SetLocalRotation(cameraRot1 * cameraRot0 * rotation);
}
