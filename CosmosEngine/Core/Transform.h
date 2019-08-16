/**
 * @file Transform.h
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

#include <DirectXMath.h>
#include <list>
#include "Component.h"

/**
 * @brief The transform of the object
 *
 */
class Transform : public Component
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
     * @return std::list<Transform*> All children of the transform
     *
     * @todo Consider add a function that get a specified child with a property
     */
    std::list<Transform*> GetChildren() const;

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
     * @return DirectX::XMVECTOR The local translation
     */
    DirectX::XMVECTOR GetLocalTranslation() const;

    /**
     * @brief Get the Local Scale
     *
     * @return DirectX::XMVECTOR The local scale
     */
    DirectX::XMVECTOR GetLocalScale() const;

    /**
     * @brief Get the Local Rotation
     *
     * @return DirectX::XMVECTOR The local rotation
     */
    DirectX::XMVECTOR GetLocalRotation() const;

    /**
     * @brief Get the Global Translation
     *
     * @return DirectX::XMVECTOR The global translation
     */
    DirectX::XMVECTOR GetGlobalTranslation();

    /**
     * @brief Get the Global Scale
     *
     * @return DirectX::XMVECTOR The global scale
     */
    DirectX::XMVECTOR GetGlobalScale();

    /**
     * @brief Get the Global Rotation
     *
     * @return DirectX::XMVECTOR The global rotation
     */
    DirectX::XMVECTOR GetGlobalRotation();

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
    void SetLocalTranslation(DirectX::XMVECTOR transVec);

    /**
     * @brief Set the Local Scale
     *
     * WARNING: Using non-uniform scale may cause some unexpected behavior!
     *
     * @param scaleVec The new local scale vector
     */
    void SetLocalScale(DirectX::XMVECTOR scaleVec);

    /**
     * @brief Set the Local Rotation
     *
     * @param rot The new local rotation quaternion
     */
    void SetLocalRotation(DirectX::XMVECTOR rot);

    /**
     * @brief Get the Local World Matrix
     *
     * @return DirectX::XMMATRIX The local world matrix
     */
    DirectX::XMMATRIX GetLocalWorldMatrix();

    /**
     * @brief Get the Local Inverse Transpose World Matrix
     *
     * @return DirectX::XMMATRIX The local inverse transpose world matrix
     */
    DirectX::XMMATRIX GetLocalInverseTransposeWorldMatrix();

    /**
     * @brief Get the Global World Matrix
     *
     * @return DirectX::XMMATRIX The global world matrix
     */
    DirectX::XMMATRIX GetGlobalWorldMatrix();

    /**
     * @brief Get the Global Inverse Transpose World Matrix
     *
     * @return DirectX::XMMATRIX The global inverse transpose world matrix
     */
    DirectX::XMMATRIX GetGlobalInverseTransposeWorldMatrix();

    /**
     * @brief Get the Global Forward unit vector of the transform
     *
     * @return DirectX::XMVECTOR The Global Forward vector
     */
    DirectX::XMVECTOR Forward();

    /**
     * @brief Get the Global Right unit vector of the transform
     *
     * @return DirectX::XMVECTOR The Global Right vector
     */
    DirectX::XMVECTOR Right();

    /**
     * @brief Get the Global Up unit vector of the transform
     *
     * @return DirectX::XMVECTOR The Global Up vector
     */
    DirectX::XMVECTOR Up();

private:
    /**
     * @brief The local translation
     *
     */
    DirectX::XMVECTOR localTranslation;
    /**
     * @brief The local scale
     *
     */
    DirectX::XMVECTOR localScale;
    /**
     * @brief The local rotation
     *
     */
    DirectX::XMVECTOR localRotation;

    /**
     * @brief The local world matrix
     *
     */
    DirectX::XMMATRIX localWorldMatrix;
    /**
     * @brief The local inverse transpose world matrix
     *
     */
    DirectX::XMMATRIX itLocalWorldMatrix;

    /**
     * @brief The global world matrix
     *
     */
    DirectX::XMMATRIX globalWorldMatrix;
    /**
     * @brief The global inverse transpose world matrix
     *
     */
    DirectX::XMMATRIX itGlobalWorldMatrix;

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
    std::list<Transform*> children;
    /**
     * @brief The parent of the transform, nullptr if the transform is the root
     *
     */
    Transform* parent;
};


#endif //GAMEENGINE_TRANSFORM_H
