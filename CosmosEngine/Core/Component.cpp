//
// Created by shuru on 8/20/2019.
//

#include "Component.h"

Component::Component(Object* owner)
        : isActive(true), object(owner), id(boost::uuids::random_generator()())
{
    started = false;
}

Component::~Component()
{
}

boost::uuids::uuid Component::GetInstanceId() const
{
    return id;
}