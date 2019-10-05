//
// Created by shuru on 8/20/2019.
//

#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform(Object* owner)
        : Component(owner)
{
    localTranslation = {0.0f, 0.0f, 0.0f};
    localScale = {1.0f, 1.0f, 1.0f};
    localRotation = glm::quat_identity<float, glm::defaultp>();

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
        UpdateWorldMat();
}

void Transform::SetParent(Transform* newParent)
{
    UpdateWorldMat();

    if (parent != nullptr)
    {
        parent->children.remove(this);
    }

    const auto tempGlobalWorldMatrix = GetGlobalWorldMatrix();
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(tempGlobalWorldMatrix, localScale, localRotation, localTranslation, skew, perspective);
    parent = nullptr;
    UpdateWorldMat();

    parent = newParent;
    if (parent != nullptr)
    {
        const glm::mat4x4 newLocal = glm::inverse(parent->GetGlobalWorldMatrix()) * globalWorldMatrix;
        glm::decompose(newLocal, localScale, localRotation, localTranslation, skew, perspective);
        UpdateWorldMat();
        parent->children.push_back(this);
    }
}

Transform* Transform::GetParent() const
{
    return parent;
}

boost::container::list<Transform*> Transform::GetChildren() const
{
    return children;
}

Transform* Transform::GetChildAt(int index)
{
    if (index >= int(children.size())) return nullptr;
    auto it = children.begin();
    for (int i = 0; i < index; ++i)
        ++it;
    return *it;
}

glm::vec3 Transform::GetLocalTranslation() const
{
    return localTranslation;
}

glm::vec3 Transform::GetLocalScale() const
{
    return localScale;
}

glm::quat Transform::GetLocalRotation() const
{
    return localRotation;
}

glm::vec3 Transform::GetGlobalTranslation()
{
    glm::vec3 translate;
    glm::quat rotate;
    glm::vec3 scale;

    const auto tempGlobalWorldMatrix = GetGlobalWorldMatrix();
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(tempGlobalWorldMatrix, scale, rotate, translate, skew, perspective);
    return translate;
}

glm::vec3 Transform::GetGlobalScale()
{
    //glm::vec4 scale = localScale;

    //if (parent != nullptr)
    //	scale = glm::vec4Multiply(scale, parent->GetGlobalScale());
    //
    //return scale;
    glm::vec3 translate;
    glm::quat rotate;
    glm::vec3 scale;

    const auto tempGlobalWorldMatrix = GetGlobalWorldMatrix();
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(tempGlobalWorldMatrix, scale, rotate, translate, skew, perspective);
    return scale;
}

glm::quat Transform::GetGlobalRotation()
{
    glm::vec3 translate;
    glm::quat rotate;
    glm::vec3 scale;

    const auto tempGlobalWorldMatrix = GetGlobalWorldMatrix();
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(tempGlobalWorldMatrix, scale, rotate, translate, skew, perspective);
    return rotate;
}

void Transform::SetLocalTranslation(const float x, const float y, const float z)
{
    localTranslation = {x, y, z};
    ShouldUpdate();
}

void Transform::SetLocalScale(const float x, const float y, const float z)
{
    localScale = {x, y, z};
    if (x != y || x != z)
        LOG_WARNING << "Non-uniform scale will result in unexpected result!";
    ShouldUpdate();
}

void Transform::SetLocalRotation(float x, float y, float z, float w)
{
    localRotation = {w, x, y, z};
    ShouldUpdate();
}

void Transform::SetLocalTranslation(glm::vec3 transVec)
{
    localTranslation = transVec;
    ShouldUpdate();
}

void Transform::SetLocalScale(glm::vec3 scaleVec)
{
    localScale = scaleVec;
    if (scaleVec[0] != scaleVec[1] || scaleVec[0] != scaleVec[2])
        LOG_WARNING << "Non-uniform scale will result in unexpected result!";
    ShouldUpdate();
}

void Transform::SetLocalRotation(glm::quat rot)
{
    localRotation = rot;
    ShouldUpdate();
}

glm::mat4x4 Transform::GetLocalWorldMatrix()
{
    if (shouldUpdate) UpdateWorldMat();
    return localWorldMatrix;
}

glm::mat4x4 Transform::GetLocalInverseTransposeWorldMatrix()
{
    if (shouldUpdate) UpdateWorldMat();
    return itLocalWorldMatrix;
}

glm::mat4x4 Transform::GetGlobalWorldMatrix()
{
    if (shouldUpdate) UpdateWorldMat();
    return globalWorldMatrix;
}

glm::mat4x4 Transform::GetGlobalInverseTransposeWorldMatrix()
{
    if (shouldUpdate) UpdateWorldMat();
    return itGlobalWorldMatrix;
}

glm::vec3 Transform::Forward()
{
    const glm::vec4 forward = {0.0f, 0.0f, 1.0f, 0.0f};
    glm::vec3 result = GetGlobalWorldMatrix() * forward;
    result = glm::normalize(result);

    return result;
}

glm::vec3 Transform::Right()
{
    const glm::vec4 right = {1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 result = GetGlobalWorldMatrix() * right;
    result = glm::normalize(result);

    return result;
}

glm::vec3 Transform::Up()
{
    const glm::vec4 up = {0.0f, 1.0f, 0.0f, 0.0f};
    glm::vec3 result = GetGlobalWorldMatrix() * up;
    result = glm::normalize(result);

    return result;
}


void Transform::UpdateWorldMat()
{
    const glm::mat4x4 t = glm::translate(glm::mat4x4(1.0f), localTranslation);
    const glm::mat4x4 r = glm::toMat4(localRotation);
    const glm::mat4x4 s = glm::scale(glm::mat4x4(1.0f), localScale);

    const glm::mat4x4 w = t * r * s;

    localWorldMatrix = w;
    itLocalWorldMatrix = glm::inverse(glm::transpose(w));

    globalWorldMatrix = w;

    if (parent != nullptr)
    {
        globalWorldMatrix = parent->GetGlobalWorldMatrix() * globalWorldMatrix;
    }

    itGlobalWorldMatrix = glm::inverse(glm::transpose(globalWorldMatrix));

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
