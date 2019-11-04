//
// Created by shuru on 11/3/2019.
//

#ifndef GAMEENGINE_SPHERECOLLIDER_H
#define GAMEENGINE_SPHERECOLLIDER_H

#include <glm/glm.hpp>

#include "../Export.h"
#include "../Core/Component.h"

class ENGINE_API SphereCollider : public Component
{
public:
    explicit SphereCollider(Object* owner);

    ~SphereCollider() final;

    bool CollideWithDiscrete(SphereCollider* other);

    bool CollideWithContinuous(SphereCollider* other, float& timeToCollision);

    bool CalculateCollisionResponse(SphereCollider* other);

    void Start() final;

    void Update(float deltaTime, float totalTime) final;

    float radius;

    float mass;

    glm::vec3 velocity;

    glm::vec3 acceleration;
};

#endif //GAMEENGINE_SPHERECOLLIDER_H
