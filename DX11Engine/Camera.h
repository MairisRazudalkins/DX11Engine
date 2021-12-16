#pragma once
#include "SceneObject.h"
class Camera : public SceneObject
{
	static Camera* activeCamera;

	DirectX::XMFLOAT4X4	projection;

public:
	Camera(Vector3 position = Vector3(), Rotator rotation = Rotator());
	virtual ~Camera() { if (this == activeCamera) { delete activeCamera; activeCamera = nullptr; } }

	static Camera* GetActiveCamera();
	static void ChangeCamera(Camera* newCam) { activeCamera = newCam; }

	DirectX::XMFLOAT4X4 GetProjection() { return projection; }
	DirectX::XMMATRIX GetCameraMatrix();

	void MoveForward(float value);
	void MoveRight(float value);

	void LookUp(float value);
	void LookRight(float value);
};