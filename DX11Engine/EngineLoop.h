#pragma once
#include "Camera.h"

class Application;
class Graphics;

class EngineLoop
{
	friend class Application;

	static EngineLoop* inst;

	Graphics* graphics;
	Application* app;

	float deltaTime;

	EngineLoop() = default;
	~EngineLoop() { delete cam; delete inst; inst = nullptr; }

	static LRESULT CALLBACK ProcessWindowsEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Initialize(Application* app, int nCmdShow);
	void SetupEngineBinds();
	void StartEngineLoop();

	void ToggleGameFocus();

	void Update();

	Camera* cam;

public:
	static EngineLoop* GetInst() { return inst != nullptr ? inst : inst = new EngineLoop(); }

	static float GetDeltaTime() { return inst != nullptr ? inst->deltaTime : 0.f; }
};