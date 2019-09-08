//
// Created by shuru on 9/8/2019.
//

#include "CameraMovement.h"
#include <CosmosEngine/Core/Object.h>

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
    glm::vec3 translation = {0.0f, 2.0f, -5.0f};
    object->transform->SetLocalTranslation(translation);

    glm::quat cameraRot0 = glm::angleAxis(glm::radians(20.0f * sinf(totalTime)), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat cameraRot1 = glm::angleAxis(glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    object->transform->SetLocalRotation(cameraRot0 * cameraRot1);
}
