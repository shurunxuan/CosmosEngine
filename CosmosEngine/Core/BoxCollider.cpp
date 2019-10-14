//
// Created by shuru on 10/13/2019.
//

#include "BoxCollider.h"
#include "Object.h"

BoxCollider::BoxCollider(Object* owner) : Collider(owner)
{
    collisionShape = new btBoxShape(btVector3(btScalar(0.5), btScalar(0.5), btScalar(0.5)));

    glm::vec3 globalPosition = object->transform->GetGlobalTranslation();
    physicsTransform.setOrigin(btVector3(globalPosition[0], globalPosition[1], globalPosition[2]));

    glm::vec3 globalScale = object->transform->GetGlobalScale();
    collisionShape->setLocalScaling(btVector3(globalScale[0], globalScale[1], globalScale[2]));

    glm::quat globalRotation = object->transform->GetGlobalRotation();
    physicsTransform.setRotation(
            btQuaternion(globalRotation[0], globalRotation[1], globalRotation[2], globalRotation[3]));
}

BoxCollider::~BoxCollider()
{
    delete collisionShape;
}

void BoxCollider::Start()
{

}

void BoxCollider::Update(float deltaTime, float totalTime)
{

}

glm::vec3 BoxCollider::GetCenter()
{
    return glm::vec3(center.getX(), center.getY(), center.getZ());
}

glm::vec3 BoxCollider::GetSize()
{
    return glm::vec3(size.getX(), size.getY(), size.getZ());
}

void BoxCollider::SetCenter(const glm::vec3& c)
{
    center.setX(c[0]);
    center.setY(c[1]);
    center.setZ(c[2]);

    glm::vec3 globalPosition = c + object->transform->GetGlobalTranslation();

    physicsTransform.setOrigin(btVector3(globalPosition[0], globalPosition[1], globalPosition[2]));
}

void BoxCollider::SetSize(const glm::vec3& s)
{
    size.setX(s[0]);
    size.setY(s[1]);
    size.setZ(s[2]);

    glm::vec3 globalScale = object->transform->GetGlobalScale();

    collisionShape->setLocalScaling(btVector3(s[0] * globalScale[0], s[1] * globalScale[1], s[2] * globalScale[2]));
}

