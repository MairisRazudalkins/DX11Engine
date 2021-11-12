#pragma once
#include "CoreMinimal.h"

class Application;

class EngineLoop
{
	friend class Application;

	static EngineLoop* inst;

	Application* app;

	EngineLoop() = default;
	~EngineLoop() { delete inst; inst = nullptr; }

	static LRESULT CALLBACK ProcessWindowsEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Initialize(Application* app, HINSTANCE* hInstance, const wchar_t* className);
	void StartEngineLoop();

public:
	static EngineLoop* GetInst() { return inst != nullptr ? inst : inst = new EngineLoop(); }

};