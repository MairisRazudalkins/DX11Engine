#pragma once
#include "CoreMinimal.h"

class Application
{
	static Application* inst;

	HINSTANCE               hInstance;
	HWND                    hwnd;
	D3D_DRIVER_TYPE         driverType;
	D3D_FEATURE_LEVEL       featureLevel;
	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;
	IDXGISwapChain*			swapChain;
	ID3D11RenderTargetView* renderTarget;

	int wHeight = 500, wWidth = 500;

	Application(HINSTANCE hInstance, int nCmdShow);
	~Application();

	void CreateAppWindow(const wchar_t* className, const wchar_t* windowTitle);
	void CreateSwapChain(); // maybe move to graphics
	void CreateRenderTarget();
	void InitializeShaderInputLayouts();

	HRESULT CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	void Callback();

	void Update();
	void Render();

	static void CreateApp(HINSTANCE hInstance, int nCmdShow, Application* app = nullptr) { if (inst == nullptr) { app = (inst = new Application(hInstance, nCmdShow)); } }
	static ID3D11Device* GetDevice() { return inst->device; }
	static ID3D11DeviceContext* GetDeviceContext() { return inst->deviceContext; }
};