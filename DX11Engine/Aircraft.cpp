#include "CoreMinimal.h"
#include "Aircraft.h"
#include "CameraArm.h"
#include "Camera.h"
#include "DirectInput.h"
#include "LitShader.h"
#include "OBJLoader.h"

Aircraft::Aircraft() : Mesh(Transform(Vector3(), Rotator(), Vector3(0.2f)), OBJLoader::Load("Assets/Models/Hercules.obj", false), new LitShader(this))
{
	camera = new Camera(Vector3(0.f, 0.f, -10.f));
	cameraArm = new CameraArm(this, camera);
	camera->SetParent(this);

	DirectInput::BindKeyboardAxis(DIK_W, 0.01f, this, &Aircraft::RotateUp);
	DirectInput::BindKeyboardAxis(DIK_S, -0.01f, this, &Aircraft::RotateUp);

	DirectInput::BindKeyboardAxis(DIK_D, 0.01f, this, &Aircraft::RotateRight);
	DirectInput::BindKeyboardAxis(DIK_A, -0.01f, this, &Aircraft::RotateRight);

	//DirectInput::BindMouseAxis(DirectMouseButton::X, 1.f, camera, &Camera::LookRight);
	//DirectInput::BindMouseAxis(DirectMouseButton::Y, 1.f, camera, &Camera::LookUp);

	DirectInput::BindMouseAxis(DirectMouseButton::X, 0.5f, this, &Aircraft::RotateCameraRight);
	DirectInput::BindMouseAxis(DirectMouseButton::Y, 0.5f, this, &Aircraft::RotateCameraUp);
}

Aircraft::~Aircraft()
{
	delete camera;
	delete cameraArm;
}

//DirectX::XMMATRIX Aircraft::GetMatrix()
//{
//	return DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
//}

//DirectX::XMMATRIX Aircraft::GetMatrix()
//{
//	auto quaternion = DirectX::XMQuaternionRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
//	DirectX::XMVECTOR fwdDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0), quaternion);
//
//	this->forward = Vector3(fwdDir.m128_f32[0], fwdDir.m128_f32[1], fwdDir.m128_f32[2]);
//
//	return DirectX::XMMatrixRotationQuaternion(quaternion) * DirectX::XMMatrixTranslation(0.f, -15.f, 15.f) * DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
//}

void Aircraft::Possess()
{
	Camera::ChangeCamera(camera);
	bIsPossessed = true;
}

void Aircraft::UnPossess()
{
	Camera::ChangeCamera(nullptr);
	bIsPossessed = false;
}

void Aircraft::Update()
{
	SceneObject::Update();
	MoveForward();
}

void Aircraft::MoveForward()
{
	if (bIsPossessed)
		SetPosition(GetPosition() + GetForward() * 0.01f);
}

void Aircraft::RotateUp(float value)
{
	if (bIsPossessed)
		SetRotation(GetRotation() + Rotator(value, 0.f, 0.f));
}

void Aircraft::RotateRight(float value)
{
	if (bIsPossessed)
		SetRotation(GetRotation() + Rotator(0.f, value, 0.f));
}

void Aircraft::LookRight(float value)
{

}

void Aircraft::LookUp(float value)
{

}

void Aircraft::RotateCameraUp(float value)
{
	//cameraArm->SetRotation(cameraArm->GetRotation() + Rotator(value, 0.f, 0.f));
}

void Aircraft::RotateCameraRight(float value)
{
	cameraArm->SetRotation(cameraArm->GetRotation() + Rotator(0.f, value, 0.f));
}