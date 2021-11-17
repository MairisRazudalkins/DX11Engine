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
	auto rotQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(position.x, position.y, position.z, 0);
	DirectX::XMVECTOR dir = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	dir = DirectX::XMVector3Rotate(dir, rotQuaternion);

	this->forward = Vector3(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2]);
	return DirectX::XMMatrixLookToLH(pos,dir,up);

	//DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//DirectX::XMVECTOR fwd = DirectX::XMVector3Rotate(DirectX::XMVectorSet(position.x, position.y, position.z, 0.f), rotationQuaternion);
	//
	//this->forward = Vector3(fwd.m128_f32[0], fwd.m128_f32[1], fwd.m128_f32[2]).Normalize();
	//
	//return DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(position.x, position.y, position.z, 0.f), fwd, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));

	//return DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y, position.z, 0.f), fwd, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
	//
	//right = Vector3(1.f, 0.f, 0.f);
	//forward = Vector3();
	//
	//Logger::ENGINE_LOG(Logger::Info, position.ToString());
}