#include "CoreMinimal.h"
#include "Camera.h"

#include "DirectInput.h"
#include "Graphics.h"
#include "EngineLoop.h"
#include "Input.h"

#include <complex>

Camera* Camera::activeCamera = nullptr;

Camera::Camera(Vector3 position, Rotator rotation) : SceneObject(Transform(position, rotation))
{
	if (activeCamera == nullptr)
		activeCamera = this;
}

Camera* Camera::GetActiveCamera()
{
	if (activeCamera == nullptr)
		return (activeCamera = new Camera());

	return activeCamera;
}

DirectX::XMMATRIX Camera::GetCameraMatrix() // TODO: FIX - Causing weird results for objects when moved in the scene. View matrix is messed up.
{
	XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Graphics::GetWindowWidth() / (FLOAT)Graphics::GetWindowHeight(), 0.01f, 500.f));

	Rotator rot = (transform.rotation).ToRadians();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);

	DirectX::XMVECTOR pos = parent != nullptr ? DirectX::XMVectorSet(parent->GetPosition().x + transform.position.x, parent->GetPosition().y + transform.position.y, parent->GetPosition().z + transform.position.z, 0) : DirectX::XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 0);
	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion));
	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]).Normalize();

	if (parent != nullptr)
	{
		return DirectX::XMMatrixLookToLH(pos, fwdDir, DirectX::XMVectorSet(up.x, up.y, up.z, 0.f));
	}

	return DirectX::XMMatrixLookToLH(pos, fwdDir, DirectX::XMVectorSet(up.x, up.y, up.z, 0.f));
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
	//Logger::ENGINE_LOG(Logger::Info, value)

	Rotator rot = GetRotation() + Rotator(value, 0.f, 0.f);
	rot.x = Math::Clamp(rot.x, -89.9f, 89.9f);
	SetRotation(rot);
}

void Camera::LookRight(float value)
{
	SetRotation((GetRotation() + Rotator(0.f, value, 0.f)));
}