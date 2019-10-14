//
// Created by shuru on 10/13/2019.
//

#ifndef GAMEENGINE_RIGIDBODY_H
#define GAMEENGINE_RIGIDBODY_H

#include "../Export.h"
#include "Component.h"
#include "Collider.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

class ENGINE_API Rigidbody : public Component
{
public:

    explicit Rigidbody(Object* owner);

    ~Rigidbody() final;

    void Start() final;

    void Update(float deltaTime, float totalTime) final;

    void SetMass(float m);

    float GetMass() const;

    void AddForce(float x, float y, float z);

    void AddForce(const glm::vec3& force);

private:

    btDefaultMotionState* motionState;

    btRigidBody* body;

    Collider* collider;

    float mass;
};

#endif //GAMEENGINE_RIGIDBODY_H
