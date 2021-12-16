#pragma once
#include "Camera.h"

class Application;
class Graphics;
class EngineController;

class EngineLoop
{
	friend class Application;

	static EngineLoop* inst;

	EngineController* engineController;

	Graphics* graphics;
	Application* app;

	float deltaTime;

	EngineLoop() = default;
	~EngineLoop() { delete inst; inst = nullptr; }

	static LRESULT CALLBACK ProcessWindowsEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Initialize(Application* app, int nCmdShow);
	void SetupEngineBinds();
	void StartEngineLoop();

	void ToggleGameFocus();

	void Update();

public:
	static EngineLoop* GetInst() { return inst != nullptr ? inst : inst = new EngineLoop(); }

	static float GetDeltaTime() { return inst != nullptr ? inst->deltaTime : 0.f; }
};