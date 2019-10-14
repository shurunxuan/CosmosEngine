//
// Created by shuru on 10/13/2019.
//

#ifndef GAMEENGINE_COLLIDER_H
#define GAMEENGINE_COLLIDER_H

#include "../Export.h"
#include "Component.h"

#include <bullet/btBulletDynamicsCommon.h>

class ENGINE_API Collider : public Component
{
public:
    friend class Rigidbody;

    explicit Collider(Object* owner);

    virtual ~Collider();

protected:

    btCollisionShape* collisionShape;

    btTransform physicsTransform;
};


#endif //GAMEENGINE_COLLIDER_H
