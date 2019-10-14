//
// Created by shuru on 10/12/2019.
//

#ifndef GAMEENGINE_PHYSICSSYSTEM_H
#define GAMEENGINE_PHYSICSSYSTEM_H

#include "../Export.h"
#include <bullet/btBulletDynamicsCommon.h>

class ENGINE_API PhysicsSystem
{
public:
    friend class Rigidbody;

    PhysicsSystem();

    ~PhysicsSystem();

    bool StartUp();

    void Update(float deltaTime);

    void Shutdown();

private:

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration;

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher;

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicsWorld;

};

ENGINE_EXTERNAL_VAR PhysicsSystem* physicsSystem;

#endif //GAMEENGINE_PHYSICSSYSTEM_H
