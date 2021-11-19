#include "CoreMinimal.h"
#include "SceneObject.h"

SceneObject::SceneObject(Vector3 position, Rotator rotation, Vector3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

DirectX::XMMATRIX SceneObject::GetMatrix()
{
	auto quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);
	DirectX::XMVECTOR upDir = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(position.x, position.y, position.z, 0);

	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);
	return DirectX::XMMatrixLookToLH(pos, fwdDir, DirectX::XMVectorSet(0, 1, 0, 0));
}