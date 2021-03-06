//
// Created by shuru on 9/8/2019.
//

#include "ObjectMovement.h"
#include <CosmosEngine/Core/Object.h>
#include <CosmosEngine/Input/InputBackend.h>
#include <CosmosEngine/App/App.h>

ObjectMovement::ObjectMovement(Object* owner) : Component(owner)
{

}

ObjectMovement::~ObjectMovement() = default;

void ObjectMovement::Start()
{

}

void ObjectMovement::Update(float deltaTime, float totalTime)
{
    auto quaternion = object->transform->GetLocalRotation();
    quaternion = quaternion * glm::angleAxis(deltaTime * glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    object->transform->SetLocalRotation(quaternion);

    auto scale = object->transform->GetLocalScale();

    float wheel = presentedInputBackend->GetAxis("Wheel");
    float zoomIn = presentedInputBackend->GetAxis("ZoomIn");
    float zoomOut = presentedInputBackend->GetAxis("ZoomOut");
    wheel += (zoomIn - zoomOut) * 0.1f;

    scale *= 1 + wheel * deltaTime * 100.0f;

    object->transform->SetLocalScale(scale);

    if (presentedInputBackend->GetButtonDown("TestMouseLeft"))
    {
        if (audioSource->Playing())
        {
            audioSource->Pause();
            LOG_DEBUG << "Playing -> Paused";
        }
        else if (audioSource->Paused())
        {
            audioSource->Play();
            LOG_DEBUG << "Paused -> Playing";
        }
        else if (audioSource->Stopped())
        {
            audioSource->Play();
            LOG_DEBUG << "Stopped -> Playing";
        }
    }

    if (presentedInputBackend->GetButtonDown("TestMouseRight"))
    {
        if (audioSource->Playing())
        {
            audioSource->Stop();
            LOG_DEBUG << "Playing -> Stopped";
        }
        else if (audioSource->Paused())
        {
            audioSource->Stop();
            LOG_DEBUG << "Paused -> Stopped";
        }
        else
        {
            LOG_DEBUG << "AudioSource Stopping.";
        }
    }
}
