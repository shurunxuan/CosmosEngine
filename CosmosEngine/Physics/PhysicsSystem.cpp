//
// Created by shuru on 10/12/2019.
//

#include "PhysicsSystem.h"
#include "../Logging/Logging.h"

PhysicsSystem* physicsSystem = nullptr;

PhysicsSystem::PhysicsSystem()
{
    collisionConfiguration = nullptr;
    dispatcher = nullptr;
    overlappingPairCache = nullptr;
    solver = nullptr;
    dynamicsWorld = nullptr;

    physicsSystem = this;
}

PhysicsSystem::~PhysicsSystem()
{

}

bool PhysicsSystem::StartUp()
{
    LOG_INFO << "Initializing Physics System";

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    return true;
}

void PhysicsSystem::Update(float deltaTime)
{
    dynamicsWorld->stepSimulation(deltaTime, 10);
}

void PhysicsSystem::Shutdown()
{
    //delete dynamics world
    delete dynamicsWorld;

    //delete solver
    delete solver;

    //delete broadphase
    delete overlappingPairCache;

    //delete dispatcher
    delete dispatcher;

    delete collisionConfiguration;

    LOG_INFO << "Physics System Shutdown Completed.";
}