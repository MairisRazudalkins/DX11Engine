#include "CoreMinimal.h"
#include "SceneObject.h"

SceneObject::SceneObject(Transform transform)
{
	this->transform.position = transform.position;
	this->transform.rotation = transform.rotation;
	this->transform.scale = transform.scale;
}

DirectX::XMMATRIX SceneObject::GetMatrix()
{
	auto quaternion = DirectX::XMQuaternionRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);

	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);

	return DirectX::XMMatrixRotationQuaternion(quaternion) * DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
}