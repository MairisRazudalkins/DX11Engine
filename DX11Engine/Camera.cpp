#include "CoreMinimal.h"
#include "Camera.h"

#include "Graphics.h"
#include "EngineLoop.h"
#include "Input.h"

Camera* Camera::activeCamera = nullptr;

Camera::Camera(Vector3 position, Rotator rotation) : SceneObject(Transform(position, rotation))
{
	if (activeCamera == nullptr)
		activeCamera = this;

	XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Graphics::GetWindowWidth() / (FLOAT)Graphics::GetWindowHeight(), 0.01f, 500.f));

	SetRotation(GetRotation() + Rotator(25.f, 0.f, 0.f));

	Input::GetInst()->BindAxis(VK_MOUSE_UP, 0.1f, this, &Camera::LookUp);
	Input::GetInst()->BindAxis(VK_MOUSE_RIGHT, 0.1f, this, &Camera::LookRight);

	Input::GetInst()->BindAxis('w', 1.f, this, &Camera::MoveForward);
	Input::GetInst()->BindAxis('s', -1.f, this, &Camera::MoveForward);

	Input::GetInst()->BindAxis('a', 1.f, this, &Camera::MoveRight);
	Input::GetInst()->BindAxis('d', -1.f, this, &Camera::MoveRight);

}

DirectX::XMMATRIX Camera::GetMatrix()
{
	Rotator rot = (transform.rotation).ToRadians();
	auto quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	
	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 0);
	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);

	return DirectX::XMMatrixLookToLH(pos, fwdDir, DirectX::XMVectorSet(0, 1, 0, 0));
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
	Rotator rot = GetRotation() + Rotator(value, 0.f, 0.f);
	rot.x = Math::Clamp(rot.x, -89.9f, 89.9f);
	SetRotation(rot);
}

void Camera::LookRight(float value)
{
	SetRotation((GetRotation() + Rotator(0.f, value, 0.f)));
}