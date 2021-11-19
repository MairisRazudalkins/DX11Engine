#include "CoreMinimal.h"
#include "Camera.h"

#include "Graphics.h"
#include "EngineLoop.h"
#include "Input.h"

Camera* Camera::activeCamera = nullptr;

Camera::Camera(Vector3 position, Rotator rotation) : SceneObject(position, rotation)
{
	if (activeCamera == nullptr)
		activeCamera = this;

	//XMStoreFloat4x4(&view, DirectX::XMMatrixLookAtLH(Eye, At, Up));
	XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Graphics::GetWindowWidth() / (FLOAT)Graphics::GetWindowHeight(), 0.01f, 500.f));

	Input::GetInst()->BindAxis(VK_MOUSE_UP, 0.01f, this, &Camera::LookUp);
	Input::GetInst()->BindAxis(VK_MOUSE_RIGHT, 0.01f, this, &Camera::LookRight);

	Input::GetInst()->BindAxis('w', 1.f, this, &Camera::MoveForward);
	Input::GetInst()->BindAxis('s', -1.f, this, &Camera::MoveForward);

	Input::GetInst()->BindAxis('a', 1.f, this, &Camera::MoveRight);
	Input::GetInst()->BindAxis('d', -1.f, this, &Camera::MoveRight);

}

void Camera::MoveForward(float value)
{
	SetPosition(GetPosition() + (GetForward() * value));
}

void Camera::MoveRight(float value)
{
	SetPosition(GetPosition() + (GetRight() * value));
}

void Camera::LookUp(float value)
{
	//Logger::ENGINE_LOG(Logger::Info, value);
	SetRotation(GetRotation() + Rotator(value, 0.f, 0.f));
}

void Camera::LookRight(float value)
{
	SetRotation(GetRotation() + Rotator(0.f, value, 0.f));
}