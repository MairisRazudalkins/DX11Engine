#pragma once
#include "Controller.h"

class Camera;

class DemoController : public Controller
{
	Camera* mainCam, *topDownCam;
	float camSpeed = 10.f;

	void SwitchCamera();
	void MoveRight(float value);
	void SetupInputBinds() override;

public:
	DemoController();
	virtual ~DemoController();
};