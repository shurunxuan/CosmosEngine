/**
 * @file Transform.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the transform component of an object
 *
 * @version 0.1
 * @date 2019/03/09
 *
 * @copyright Copyright (c) 2019
 *
 */


#ifndef GAMEENGINE_TRANSFORM_H
#define GAMEENGINE_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <boost/container/list.hpp>
#include "../Logging/Logging.h"
#include "Component.h"
#include "../Export.h"

/**
 * @brief The transform of the object
 *
 */
class ENGINE_API Transform : public Component
{
public:
    friend class Camera;


    /**
     * @brief Construct a new Transform
     *
     * @param owner The object that owns the Transform
     */
    explicit Transform(Object* owner);

    /**
     * @brief Destroy the Transform object
     *
     */
    ~Transform() final;

    /**
     * @brief Does nothing
     *
     */
    void Start() override;

    /**
     * @brief If required, update the vectors and matrices of the transform
     *
     * @param deltaTime The time a frame costs
     * @param totalTime The total time from the beginning of the application
     */
    void Update(float deltaTime, float totalTime) override final;

    /**
     * @brief Set a new parent
     *
     * @param newParent The new parent, can be nullptr to remove the parent
     */
    void SetParent(Transform* newParent);

    /**
     * @brief Get the parent Transform
     *
     * @return Transform* The pointer of the parent transform, nullptr if there's no parent
     */
    Transform* GetParent() const;

    /**
     * @brief Get all children of the transform
     *
     * @return boost::container::list<Transform*> All children of the transform
     *
     * @todo Consider add a function that get a specified child with a property
     */
    boost::container::list<Transform*> GetChildren() const;

    /**
     * @brief Get a child of the transform
     *
     * @param index The position of the child to be get
     *
     * @return Transform* The child at a specified position
     */
    Transform* GetChildAt(int index);

    /**
     * @brief Get the Local Translation
     *
     * @return glm::vec3 The local translation
     */
    glm::vec3 GetLocalTranslation() const;

    /**
     * @brief Get the Local Scale
     *
     * @return glm::vec3 The local scale
     */
    glm::vec3 GetLocalScale() const;

    /**
     * @brief Get the Local Rotation
     *
     * @return glm::quat The local rotation
     */
    glm::quat GetLocalRotation() const;

    /**
     * @brief Get the Global Translation
     *
     * @return glm::vec3 The global translation
     */
    glm::vec3 GetGlobalTranslation();

    /**
     * @brief Get the Global Scale
     *
     * @return glm::vec3 The global scale
     */
    glm::vec3 GetGlobalScale();

    /**
     * @brief Get the Global Rotation
     *
     * @return glm::quat The global rotation
     */
    glm::quat GetGlobalRotation();

    /**
     * @brief Set the Local Translation
     *
     * @param x The translation along the x axis
     * @param y The translation along the y axis
     * @param z The translation along the z axis
     */
    void SetLocalTranslation(float x, float y, float z);

    /**
     * @brief Set the Local Scale
     *
     * WARNING: Using non-uniform scale may cause some unexpected behavior!
     *
     * @param x The scale of the x axis
     * @param y The scale of the y axis
     * @param z The scale of the z axis
     */
    void SetLocalScale(float x, float y, float z);

    /**
     * @brief Set the Local Rotation
     *
     * @param x The x component of the new rotation quaternion
     * @param y The y component of the new rotation quaternion
     * @param z The z component of the new rotation quaternion
     * @param w The w component of the new rotation quaternion
     */
    void SetLocalRotation(float x, float y, float z, float w);

    /**
     * @brief Set the Local Translation
     *
     * @param transVec The new local translation vector
     */
    void SetLocalTranslation(glm::vec3 transVec);

    /**
     * @brief Set the Local Scale
     *
     * WARNING: Using non-uniform scale may cause some unexpected behavior!
     *
     * @param scaleVec The new local scale vector
     */
    void SetLocalScale(glm::vec3 scaleVec);

    /**
     * @brief Set the Local Rotation
     *
     * @param rot The new local rotation quaternion
     */
    void SetLocalRotation(glm::quat rot);

    /**
     * @brief Get the Local World Matrix
     *
     * @return glm::mat4x4 The local world matrix
     */
    glm::mat4x4 GetLocalWorldMatrix();

    /**
     * @brief Get the Local Inverse Transpose World Matrix
     *
     * @return glm::mat4x4 The local inverse transpose world matrix
     */
    glm::mat4x4 GetLocalInverseTransposeWorldMatrix();

    /**
     * @brief Get the Global World Matrix
     *
     * @return glm::mat4x4 The global world matrix
     */
    glm::mat4x4 GetGlobalWorldMatrix();

    /**
     * @brief Get the Global Inverse Transpose World Matrix
     *
     * @return glm::mat4x4 The global inverse transpose world matrix
     */
    glm::mat4x4 GetGlobalInverseTransposeWorldMatrix();

    /**
     * @brief Get the Global Forward unit vector of the transform
     *
     * @return glm::vec3 The Global Forward vector
     */
    glm::vec3 Forward();

    /**
     * @brief Get the Global Right unit vector of the transform
     *
     * @return glm::vec3 The Global Right vector
     */
    glm::vec3 Right();

    /**
     * @brief Get the Global Up unit vector of the transform
     *
     * @return glm::vec3 The Global Up vector
     */
    glm::vec3 Up();

private:
    /**
     * @brief The local translation
     *
     */
    glm::vec3 localTranslation;
    /**
     * @brief The local scale
     *
     */
    glm::vec3 localScale;
    /**
     * @brief The local rotation
     *
     */
    glm::quat localRotation;

    /**
     * @brief The local world matrix
     *
     */
    glm::mat4x4 localWorldMatrix;
    /**
     * @brief The local inverse transpose world matrix
     *
     */
    glm::mat4x4 itLocalWorldMatrix;

    /**
     * @brief The global world matrix
     *
     */
    glm::mat4x4 globalWorldMatrix;
    /**
     * @brief The global inverse transpose world matrix
     *
     */
    glm::mat4x4 itGlobalWorldMatrix;

    /**
     * @brief Indicates if the vectors/matrices should be updated
     *
     */
    bool shouldUpdate;

    /**
     * @brief Update the world matrices
     *
     */
    void UpdateWorldMat();

    /**
     * @brief Request the update of this transform and all children transforms recursively
     *
     */
    void ShouldUpdate();

    /**
     * @brief All children of the transform
     *
     */
    boost::container::list<Transform*> children;
    /**
     * @brief The parent of the transform, nullptr if the transform is the root
     *
     */
    Transform* parent;
};


#endif //GAMEENGINE_TRANSFORM_H
