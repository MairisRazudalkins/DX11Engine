#pragma once
#include "Mesh.h"

class Camera;
class CameraArm;

class Aircraft : public Mesh // Could inherit from a pawn class and have base functions like possess.
{
	CameraArm* cameraArm;
	Camera* camera;

	bool bIsPossessed = false;

	void MoveForward();
	void RotateUp(float value);
	void RotateRight(float value);

	void LookRight(float value);
	void LookUp(float value);

	void RotateCameraUp(float value);
	void RotateCameraRight(float value);

public:
	Aircraft();
	~Aircraft();

	//DirectX::XMMATRIX GetMatrix() override;

	void Possess();
	void UnPossess();
	void Update() override;
};