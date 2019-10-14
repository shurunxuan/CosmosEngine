//
// Created by shuru on 10/13/2019.
//

#include "Rigidbody.h"
#include "Object.h"
#include "../Physics/PhysicsSystem.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Rigidbody::Rigidbody(Object* owner) : Component(owner)
{
    motionState = nullptr;
    body = nullptr;
    collider = nullptr;
    mass = 0.0f;
}

Rigidbody::~Rigidbody()
{
    delete motionState;
    delete body;
}

void Rigidbody::Start()
{
    // TODO: use a compound shape for this
    collider = object->GetComponent<Collider>();
    btCollisionShape * collisionShape = nullptr;

    btVector3 localInertia(0, 0, 0);
    if (collider != nullptr)
    {
        if (mass != 0.0f)
        {
            collider->collisionShape->calculateLocalInertia(mass, localInertia);
        }

        motionState = new btDefaultMotionState(collider->physicsTransform);
        collisionShape = collider->collisionShape;
    }
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
    body = new btRigidBody(rbInfo);

    physicsSystem->dynamicsWorld->addRigidBody(body);
}

void Rigidbody::Update(float deltaTime, float totalTime)
{
    if (body->wantsSleeping()) return;
    btTransform trans;
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(trans);
    }
    else
    {
        trans = body->getWorldTransform();
    }

    glm::mat4 rigidbodyTransform;
    trans.getOpenGLMatrix(glm::value_ptr(rigidbodyTransform));

    // TODO: This can be done in transform (set global properties)
    glm::mat4 globalTransform = object->transform->GetGlobalWorldMatrix();

    glm::mat4 originalLocalTransform = object->transform->GetLocalWorldMatrix();

    glm::mat4 transformOffset = glm::inverse(globalTransform) * rigidbodyTransform;

    glm::mat4 newLocalTransform = originalLocalTransform * transformOffset;

    glm::vec3 newLocalTranslation;
    glm::quat newLocalRotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(newLocalTransform, scale, newLocalRotation, newLocalTranslation, skew, perspective);

    object->transform->SetLocalTranslation(newLocalTranslation);
    object->transform->SetLocalRotation(newLocalRotation);
}

void Rigidbody::SetMass(float m)
{
    mass = m;
}

float Rigidbody::GetMass() const
{
    return mass;
}

void Rigidbody::AddForce(float x, float y, float z)
{
    body->setActivationState(DISABLE_DEACTIVATION);
    body->applyCentralImpulse(btVector3(x, y, z));
}

void Rigidbody::AddForce(const glm::vec3& force)
{
    AddForce(force[0], force[1], force[2]);
}