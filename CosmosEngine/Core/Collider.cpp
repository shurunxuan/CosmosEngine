//
// Created by shuru on 10/13/2019.
//

#include "Collider.h"

Collider::Collider(Object* owner) : Component(owner)
{
    collisionShape = nullptr;

    physicsTransform.setIdentity();
}

Collider::~Collider()
{

}
