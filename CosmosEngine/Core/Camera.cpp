//
// Created by 西村智 on 8/30/19.
//

#include "Camera.h"
#include "../Logging/Logging.h"

Camera::Camera(Scene* owner, boost::container::string name)
        : Object(owner, name)
{
    //skybox = nullptr;
    //AddComponent<AudioListener>();
    //UpdateViewMatrix();

    //UpdateProjectionMatrix(screenWidth, screenHeight, 3.14159265f / 4.0f);

    fov = glm::radians(45.0f);
    zNear = 0.1f;
    zFar = 1000.0f;
}


Camera::~Camera()
{
    //delete skybox;
}

//void Camera::SetSkybox(ID3D11Device* d, ID3D11DeviceContext* c, const boost::container::wstring& cubeMapFile,
//                              const boost::container::wstring& irradianceMapFile = L"")
//{
//    delete skybox;
//    skybox = new Skybox(d, c, cubeMapFile, irradianceMapFile);
//}

//Skybox* Camera::GetSkybox()
//{
//    return skybox;
//}

void Camera::UpdateProjectionMatrix()
{
    // Create the Projection matrix
    // - This should match the window's aspect ratio, and also update anytime
    //    the window resizes (which is already happening in OnResize() below)
    projectionMatrix = glm::perspective(fov, width / height, zNear, zFar);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return projectionMatrix;
}

//glm::mat4 Camera::GetViewMatrix() const
//{
//    //return DirectX::XMMatrixLookToLH(transform->GetGlobalTranslation(), transform->Forward(), transform->Up());
//}

float Camera::GetNearClip() const
{
    return zNear;
}

float Camera::GetFarClip() const
{
    return zFar;
}

//DirectX::XMVECTOR Camera::ScreenToWorldPoint(float x, float y) const
//{
//    const float ndcX = 2.0f * x / width - 1;
//    const float ndcY = -2.0f * y / height + 1;
//    const DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(XMMatrixTranspose(GetViewMatrix()), XMMatrixTranspose(projectionMatrix));
//    const DirectX::XMMATRIX invViewProj = XMMatrixInverse(nullptr, viewProj);
//    const DirectX::XMVECTOR screenPoint = DirectX::XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
//    DirectX::XMVECTOR worldPos = DirectX::XMVector3TransformCoord(screenPoint, invViewProj);
//    return worldPos;
//
//}

void Camera::Resize(float clientWidth, float clientHeight)
{
    width = clientWidth;
    height = clientHeight;

    UpdateProjectionMatrix();
}

void Camera::SetFov(float fov)
{
    this->fov = fov;

    UpdateProjectionMatrix();
}

void Camera::SetNearPlane(float zNear)
{
    this->zNear = zNear;

    UpdateProjectionMatrix();
}

void Camera::SetFarPlane(float zFar)
{
    this->zFar = zFar;

    UpdateProjectionMatrix();
}

//DirectX::XMFLOAT2 Camera::WorldPositionToScreenPoint(DirectX::XMVECTOR worldPosition) const
//{
//	const DirectX::XMMATRIX view = XMMatrixInverse(nullptr, XMMatrixTranspose(transform->GetGlobalWorldMatrix()));
//	const DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(XMMatrixTranspose(projectionMatrix), view);
//	worldPosition = DirectX::XMVector3Transform(worldPosition, viewProj);
//	const float x = roundf(((worldPosition.m128_f32[0] + 1) / 2.0f) *
//		width);
//	const float y = roundf(((1 - worldPosition.m128_f32[1]) / 2.0f) *
//		height);
//	return {x, y};
//}
