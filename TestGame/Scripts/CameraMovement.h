//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_CAMERAMOVEMENT_H
#define GAMEENGINE_CAMERAMOVEMENT_H

#include <CosmosEngine/Core/Component.h>

class CameraMovement
        : public Component
{
public:
    explicit CameraMovement(Object* owner);

    ~CameraMovement() final;

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

#endif //GAMEENGINE_CAMERAMOVEMENT_H
