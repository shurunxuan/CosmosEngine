//
// Created by shuru on 8/15/2019.
//

#include "Transform.h"
#include "../Logging/Logging.h"

Transform::Transform(Object* owner)
        : Component(owner)
{
    localTranslation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    localScale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    localRotation = DirectX::XMQuaternionIdentity();

    parent = nullptr;

    shouldUpdate = true;

    UpdateWorldMat();
}

Transform::~Transform()
{
}

void Transform::Start()
{
}

void Transform::Update(float deltaTime, float totalTime)
{
    if (shouldUpdate)
    {
        UpdateWorldMat();
    }
}

void Transform::SetParent(Transform* newParent)
{
    UpdateWorldMat();

    if (parent != nullptr)
    {
        parent->children.remove(this);
    }

    DirectX::XMMatrixDecompose(&localScale, &localRotation, &localTranslation,
                               XMMatrixTranspose(GetGlobalWorldMatrix()));
    parent = nullptr;
    UpdateWorldMat();

    parent = newParent;
    if (parent != nullptr)
    {
        const DirectX::XMMATRIX newLocal = DirectX::XMMatrixMultiply(XMMatrixTranspose(globalWorldMatrix),
                                                                     XMMatrixInverse(nullptr, XMMatrixTranspose(
                                                                             parent->GetGlobalWorldMatrix())));
        DirectX::XMMatrixDecompose(&localScale, &localRotation, &localTranslation, newLocal);
        UpdateWorldMat();
        parent->children.push_back(this);
    }
}

Transform* Transform::GetParent() const
{
    return parent;
}

std::list<Transform*> Transform::GetChildren() const
{
    return children;
}

Transform* Transform::GetChildAt(int index)
{
    if (index >= int(children.size()))
    { return nullptr; }
    auto it = children.begin();
    for (int i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

DirectX::XMVECTOR Transform::GetLocalTranslation() const
{
    return localTranslation;
}

DirectX::XMVECTOR Transform::GetLocalScale() const
{
    return localScale;
}

DirectX::XMVECTOR Transform::GetLocalRotation() const
{
    return localRotation;
}

DirectX::XMVECTOR Transform::GetGlobalTranslation()
{
    DirectX::XMVECTOR translate;
    DirectX::XMVECTOR rotate;
    DirectX::XMVECTOR scale;

    const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

    DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
    return translate;
}

DirectX::XMVECTOR Transform::GetGlobalScale()
{
    //DirectX::XMVECTOR scale = localScale;

    //if (parent != nullptr)
    //	scale = DirectX::XMVectorMultiply(scale, parent->GetGlobalScale());
    //
    //return scale;
    DirectX::XMVECTOR translate;
    DirectX::XMVECTOR rotate;
    DirectX::XMVECTOR scale;

    const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

    DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
    return scale;
}

DirectX::XMVECTOR Transform::GetGlobalRotation()
{
    DirectX::XMVECTOR translate;
    DirectX::XMVECTOR rotate;
    DirectX::XMVECTOR scale;

    const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

    DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
    return rotate;
}

void Transform::SetLocalTranslation(const float x, const float y, const float z)
{
    localTranslation = DirectX::XMVectorSet(x, y, z, 0.0f);
    ShouldUpdate();
}

void Transform::SetLocalScale(const float x, const float y, const float z)
{
    localScale = DirectX::XMVectorSet(x, y, z, 0.0f);
    if (x != y || x != z)
        LOG_WARNING << "Non-uniform scale will result in unexpected result!";
    ShouldUpdate();
}

void Transform::SetLocalRotation(float x, float y, float z, float w)
{
    localRotation = DirectX::XMVectorSet(x, y, z, w);
    ShouldUpdate();
}

void Transform::SetLocalTranslation(DirectX::XMVECTOR transVec)
{
    localTranslation = transVec;
    ShouldUpdate();
}

void Transform::SetLocalScale(DirectX::XMVECTOR scaleVec)
{
    localScale = scaleVec;
    if (DirectX::XMVectorGetByIndex(scaleVec, 0) != DirectX::XMVectorGetByIndex(scaleVec, 1) ||
        DirectX::XMVectorGetByIndex(scaleVec, 0) != DirectX::XMVectorGetByIndex(scaleVec, 2))
        LOG_WARNING << "Non-uniform scale will result in unexpected result!";
    ShouldUpdate();
}

void Transform::SetLocalRotation(DirectX::XMVECTOR rot)
{
    localRotation = rot;
    ShouldUpdate();
}

DirectX::XMMATRIX Transform::GetLocalWorldMatrix()
{
    if (shouldUpdate)
    { UpdateWorldMat(); }
    return localWorldMatrix;
}

DirectX::XMMATRIX Transform::GetLocalInverseTransposeWorldMatrix()
{
    if (shouldUpdate)
    { UpdateWorldMat(); }
    return itLocalWorldMatrix;
}

DirectX::XMMATRIX Transform::GetGlobalWorldMatrix()
{
    if (shouldUpdate)
    { UpdateWorldMat(); }
    return globalWorldMatrix;
}

DirectX::XMMATRIX Transform::GetGlobalInverseTransposeWorldMatrix()
{
    if (shouldUpdate)
    { UpdateWorldMat(); }
    return itGlobalWorldMatrix;
}

DirectX::XMVECTOR Transform::Forward()
{
    const DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMVECTOR result = DirectX::XMVector4Transform(forward, XMMatrixTranspose(GetGlobalWorldMatrix()));
    result = DirectX::XMVector3Normalize(result);

    return result;
}

DirectX::XMVECTOR Transform::Right()
{
    const DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR result = DirectX::XMVector4Transform(right, XMMatrixTranspose(GetGlobalWorldMatrix()));
    result = DirectX::XMVector3Normalize(result);

    return result;
}

DirectX::XMVECTOR Transform::Up()
{
    const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR result = DirectX::XMVector4Transform(up, XMMatrixTranspose(GetGlobalWorldMatrix()));
    result = DirectX::XMVector3Normalize(result);

    return result;
}


void Transform::UpdateWorldMat()
{
    const DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(localTranslation);
    const DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(localRotation);
    const DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(localScale);

    const DirectX::XMMATRIX w = s * r * t;
    localWorldMatrix = DirectX::XMMatrixTranspose(w);
    itLocalWorldMatrix = DirectX::XMMatrixInverse(nullptr, w);

    globalWorldMatrix = w;

    if (parent != nullptr)
    {
        globalWorldMatrix = DirectX::XMMatrixMultiply(globalWorldMatrix,
                                                      XMMatrixTranspose(parent->GetGlobalWorldMatrix()));
    }

    itGlobalWorldMatrix = DirectX::XMMatrixInverse(nullptr, globalWorldMatrix);
    globalWorldMatrix = XMMatrixTranspose(globalWorldMatrix);

    shouldUpdate = false;
}

void Transform::ShouldUpdate()
{
    for (Transform* child : children)
    {
        child->ShouldUpdate();
    }

    shouldUpdate = true;
}