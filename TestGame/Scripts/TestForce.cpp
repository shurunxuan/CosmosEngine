//
// Created by shuru on 10/13/2019.
//

#include "TestForce.h"
#include <CosmosEngine/Core/Object.h>
#include <CosmosEngine/Input/InputBackend.h>

TestForce::TestForce(Object* owner) : Component(owner)
{
    rb = nullptr;
}

TestForce::~TestForce()
{

}

void TestForce::Start()
{
    rb = object->GetComponent<Rigidbody>();
}

void TestForce::Update(float deltaTime, float totalTime)
{
    if (presentedInputBackend->GetButtonDown("ApplyForce"))
    {
        if (rb != nullptr)
        {
            rb->AddForce(0.0f, 10.0f, 0.0f);
        }
    }
}
