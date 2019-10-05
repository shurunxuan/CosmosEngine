//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_OBJECTMOVEMENT_H
#define GAMEENGINE_OBJECTMOVEMENT_H

#include <CosmosEngine/Core/Component.h>

class ObjectMovement
        : public Component
{
public:
    explicit ObjectMovement(Object* owner);

    ~ObjectMovement() final;

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
};

#endif //GAMEENGINE_OBJECTMOVEMENT_H
