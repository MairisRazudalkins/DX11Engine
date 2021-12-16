#include "CoreMinimal.h"
#include "SceneObject.h"

SceneObject::SceneObject(Transform transform)
{
	this->transform.position = transform.position;
	this->transform.rotation = transform.rotation;
	this->transform.scale = transform.scale;
}

DirectX::XMMATRIX SceneObject::GetMatrix() // ALL SO SCUFFED!!!!
{
	Rotator rot = transform.rotation.ToRadians();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);

	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);
	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);

	DirectX::XMMATRIX objMatrix = DirectX::XMMatrixRotationQuaternion(quaternion) * DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

	return parent != nullptr ? objMatrix * parent->GetMatrix() : objMatrix;
}

DirectX::XMFLOAT4X4 SceneObject::GetWorldMatrix()
{
	//Rotator rot = transform.rotation.ToRadians();
	//DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//
	//if (parent != nullptr)
	//	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixRotationQuaternion(quaternion) * DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z) * DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * DirectX::XMLoadFloat4x4(&parent->GetWorldMatrix()));// * DirectX::XMMatrixRotationQuaternion(quaternion) * DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z) * DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z));
	//else
		DirectX::XMStoreFloat4x4(&worldMatrix, GetMatrix());

	//DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);
	//this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);

	return worldMatrix;

	//
	//return XMLoadFloat4x4(&worldMatrix);
}