//
// Created by shuru on 10/13/2019.
//

#ifndef GAMEENGINE_TESTFORCE_H
#define GAMEENGINE_TESTFORCE_H

#include <CosmosEngine/Core/Component.h>
#include <CosmosEngine/Core/Rigidbody.h>

class TestForce
        : public Component
{
public:
    explicit TestForce(Object* owner);

    ~TestForce() final;

    /**
     * @brief Will be called as soon as the component is created
     *
     */
    void Start() final;

    /**
     * @brief Will be called when the object updates
     *
     * @param deltaTime The time that a frame costs
     * @param totalTime The total time from the beginning of the application
     */
    void Update(float deltaTime, float totalTime) final;

private:
    Rigidbody* rb;
};

#endif //GAMEENGINE_TESTFORCE_H
