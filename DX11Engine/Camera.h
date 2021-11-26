#pragma once
#include "SceneObject.h"
class Camera : public SceneObject
{
	static Camera* activeCamera;

	DirectX::XMFLOAT4X4	projection;

	void MoveForward(float value);
	void MoveRight(float value);

	void LookUp(float value);
	void LookRight(float value);

public:
	Camera(Vector3 position = Vector3(), Rotator rotation = Rotator());
	virtual ~Camera() = default;

	static Camera* GetActiveCamera() { return activeCamera; }
	DirectX::XMFLOAT4X4 GetProjection() { return projection; }

	DirectX::XMMATRIX GetMatrix() override;
};