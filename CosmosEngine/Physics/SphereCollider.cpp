//
// Created by shuru on 11/3/2019.
//

#include "../Core/Object.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(Object* owner) : Component(owner)
{
    radius = 0;
    mass = 0;
    velocity = glm::vec3(0, 0, 0);
    acceleration = glm::vec3(0, 0, 0);
}

SphereCollider::~SphereCollider()
{

}

bool SphereCollider::CollideWithDiscrete(SphereCollider* other)
{
    float length = glm::distance(object->transform->GetGlobalTranslation(),
                                 other->object->transform->GetGlobalTranslation());

    return length <= radius + other->radius;

}

bool SphereCollider::CollideWithContinuous(SphereCollider* other, float& timeToCollision)
{
    glm::vec3 s = object->transform->GetGlobalTranslation() -
                  other->object->transform->GetGlobalTranslation(); // vector between the centers of each sphere
    glm::vec3 v = velocity - other->velocity; // relative velocity between spheres
    float r = radius + other->radius;

    float c1 = glm::dot(s, s) - r * r; // if negative, they overlap
    if (c1 < 0.0) // if true, they already overlap
    {
        // This is bad ... we need to correct this by moving them a tiny fraction from each other
        //a->pos +=
        timeToCollision = .0;
        return true;
    }

    float a1 = glm::dot(v, v);
    if (a1 < 0.00001f)
        return false; // does not move towards each other

    float b1 = glm::dot(v, s);
    if (b1 >= 0.0)
        return false; // does not move towards each other

    float d1 = b1 * b1 - a1 * c1;
    if (d1 < 0.0)
        return false; // no real roots ... no collision

    timeToCollision = (-b1 - sqrtf(d1)) / a1;

    return true;
}

bool SphereCollider::CalculateCollisionResponse(SphereCollider* other)
{
    float m1, m2, x1, x2;
    glm::vec3 v1temp, v1, v2, v1x, v2x, v1y, v2y,
            x(object->transform->GetGlobalTranslation() -
              other->object->transform->GetGlobalTranslation());

    x = glm::normalize(x);
    v1 = velocity;
    x1 = glm::dot(x, v1);
    v1x = x * x1;
    v1y = v1 - v1x;
    m1 = mass;

    x = x * -1.0f;
    v2 = other->velocity;
    x2 = glm::dot(x, v2);
    v2x = x * x2;
    v2y = v2 - v2x;
    m2 = other->mass;

    velocity = v1x * (m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y;

    return true;
}

void SphereCollider::Start()
{

}

void SphereCollider::Update(float deltaTime, float totalTime)
{
    auto position = object->transform->GetGlobalTranslation();
    position += velocity * deltaTime;

    if (position.x < -20.0f + radius)
    {
        position.x = -20.0f + radius;
        velocity.x = -velocity.x * 0.9f;
    }
    else if (position.x > 20.0f - radius)
    {
        position.x = 20.0f - radius;
        velocity.x = -velocity.x * 0.9f;
    }
    if (position.y < -20.0f + radius)
    {
        position.y = -20.0f + radius;
        velocity.y = -velocity.y * 0.9f;
    }
    else if (position.y > 20.0f - radius)
    {
        position.y = 20.0f - radius;
        velocity.y = -velocity.y * 0.9f;
    }
    if (position.z < -20.0f + radius)
    {
        position.z = -20.0f + radius;
        velocity.z = -velocity.z * 0.9f;
    }
    else if (position.z > 20.0f - radius)
    {
        position.z = 20.0f - radius;
        velocity.z = -velocity.z * 0.9f;
    }

    object->transform->SetLocalTranslation(position);
}
