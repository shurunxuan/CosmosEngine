/**
 * @file Camera.h
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the camera object
 *
 * @version 0.1
 * @date 2019/03/11
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include <boost/container/string.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "../Logging/Logging.h"
#include "Object.h"
//#include "Skybox.h"
//#include "AudioListener.hpp"

// TODO: Skybox not present

class Scene;

/**
 * @brief Camera
 *
 */
class ENGINE_API Camera final
        : public Object
{
public:
    /**
     * @brief Construct a new Camera object
     *
     * @param owner The scene that owns the camera
     * @param name The name of the camera, default to MainCamera
     */
    Camera(Scene* owner, boost::container::string name = "MainCamera");

    /**
     * @brief Destroy the Camera object
     *
     */
    ~Camera();

    /**
     * @brief Create a skybox from file
     *
     * @param d Direct3D 11 device
     * @param c Direct3D 11 device context
     * @param cubeMapFile The cubemap of the skybox
     * @param irradianceMapFile The irradiance map of the cubemap
     */
    //void SetSkybox(ID3D11Device* d, ID3D11DeviceContext* c, const boost::container::wstring& cubeMapFile, const boost::container::wstring& irradianceMapFile);
    /**
     * @brief Get the Skybox object
     *
     * @return Skybox* A pointer points to the skybox
     */
    //Skybox* GetSkybox();

    /**
     * @brief Resize the client
     *
     * @param clientWidth The screen width
     * @param clientHeight The screen height
     */
    void Resize(float clientWidth, float clientHeight);

    /**
     * @brief Set the Field of View
     *
     * @param fov Field of view
     */
    void SetFov(float fov);

    /**
    * @brief Set the near plane
    *
    * @param zNear nearPlane
    */
    void SetNearPlane(float zNear);

    /**
     * @brief Set the far plane
     *
     * @param zFar far plane
     */
    void SetFarPlane(float zFar);

    /**
     * @brief Get the projection matrix
     *
     * @return glm::mat4 The reference of the projection matrix
     */
    glm::mat4 GetProjectionMatrix();

    /**
     * @brief Get the View matrix
     *
     * @return glm::mat4 The reference of the projection matrix
     */
//    glm::mat4 GetViewMatrix() const;

    /**
     * @brief Get the Near Clip of the camera
     *
     * @return float Near clip
     *
     * @todo Currently, this only returns 0.1f
     */
    float GetNearClip() const;

    /**
     * @brief Get the Far Clip of the camera
     *
     * @return float Far clip
     *
     * @todo Currently, this only returns 1000.0f
     */
    float GetFarClip() const;

    /**
     * @brief Get the world point from mouse cursor position (near plane in Z)
     *
     * @param x X position of the mouse cursor in screen
     * @param y Y position of the mouse cursor in screen
     *
     * @return glm::vec3 The world position
     */
//    glm::vec3 ScreenToWorldPoint(float x, float y) const;

    //DirectX::XMFLOAT2 WorldPositionToScreenPoint(DirectX::XMVECTOR worldPosition) const;

private:
    /**
     * @brief The projection matrix
     *
     */
    glm::mat4 projectionMatrix;
    /**
     * @brief The skybox
     *
     */
    //Skybox* skybox;

    /**
     * @brief Client width
     */
    float width;

    /**
     * @brief Client height
     */
    float height;

    /**
     * @brief Field of View
     */
    float fov;

    /**
     * @brief Near plane
     */
    float zNear;

    /**
     * @brief Far plane
     */
    float zFar;

    /**
     * @brief Update the projection matrix
     */
    void UpdateProjectionMatrix();
};

#endif //GAMEENGINE_CAMERA_H
