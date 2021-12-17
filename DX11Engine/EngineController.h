#pragma once
#include "Controller.h"

class Camera;

class EngineController : public Controller
{
	Camera* engineCamera;
	Camera* gameCamera;

	float camSpeed = 30.f;

	void Trap();
	void Release();

	void ToggleFocus();
	void EngineFocus();

	void SetGlobalRasterToWireframe();
	void SetResetGlobalRasterizer();

	float mouseSensitivity = 0.2f;

protected:
	virtual void SetupInputBinds() override;

public:
	EngineController();
	virtual ~EngineController();
};