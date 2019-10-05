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

void CameraMovement::Start()
{

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
