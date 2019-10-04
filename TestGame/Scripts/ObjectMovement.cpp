//
// Created by shuru on 9/8/2019.
//

#include "ObjectMovement.h"
#include <CosmosEngine/Core/Object.h>
#include <CosmosEngine/Input/InputBackend.h>

ObjectMovement::ObjectMovement(Object* owner) : Component(owner)
{

}

ObjectMovement::~ObjectMovement() = default;

void ObjectMovement::Start()
{

}

void ObjectMovement::Update(float deltaTime, float totalTime)
{
    if (movementType == 0)
    {
        auto quaternion = object->transform->GetLocalRotation();
        quaternion = quaternion * glm::angleAxis(deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        object->transform->SetLocalRotation(quaternion);

        auto position = object->transform->GetLocalTranslation();
        position.y = -0.5f;
        object->transform->SetLocalTranslation(position);
    }
    else
    {
        auto quaternion = object->transform->GetLocalRotation();
        quaternion = quaternion * glm::angleAxis(deltaTime * glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        object->transform->SetLocalRotation(quaternion);

        auto position = object->transform->GetLocalTranslation();

        float horizontal = presentedInputBackend->GetAxis("Horizontal");
        float vertical = presentedInputBackend->GetAxis("Vertical");

        position.x += horizontal * deltaTime;
        position.y += vertical * deltaTime;

        object->transform->SetLocalTranslation(position);
    }
}
