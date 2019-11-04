//
// Created by shuru on 11/3/2019.
//

#ifndef GAMEENGINE_PHYSICSSYSTEM_H
#define GAMEENGINE_PHYSICSSYSTEM_H

#include "../Export.h"

class ENGINE_LOCAL PhysicsSystem
{
public:

    PhysicsSystem();

    ~PhysicsSystem();

    bool StartUp();

    void Update(float deltaTime, float totalTime);

    void Shutdown();
};

#endif //GAMEENGINE_PHYSICSSYSTEM_H
