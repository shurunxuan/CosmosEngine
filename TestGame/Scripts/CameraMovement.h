//
// Created by shuru on 9/8/2019.
//

#ifndef GAMEENGINE_CAMERAMOVEMENT_H
#define GAMEENGINE_CAMERAMOVEMENT_H

#include <CosmosEngine/Core/Component.h>

#include <CosmosEngine/Serialization/Serialization.h>

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

private:
    SERIALIZABLE_CLASS;

    SERIALIZER
    {
        ADD_FIELD(horizontalSpeed);
        ADD_FIELD(verticalSpeed);
    };


    float horizontalSpeed = 12.0f;
    float verticalSpeed = 3.0f;
};

#endif //GAMEENGINE_CAMERAMOVEMENT_H
