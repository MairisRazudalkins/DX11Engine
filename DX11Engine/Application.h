#pragma once

class Application
{
	static Application*		inst;

	HINSTANCE               hInstance;
	HWND                    hwnd;

	Application(HINSTANCE hInstance, int nCmdShow);
	~Application();

	void RegisterWin(HINSTANCE& hInstance, const wchar_t* className);
	void CreateAppWindow(const wchar_t* className, const wchar_t* windowTitle);

	HRESULT CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	static void CreateApp(HINSTANCE hInstance, int nCmdShow, Application* app = nullptr) { if (inst == nullptr) { app = new Application(hInstance, nCmdShow); } }

	static HINSTANCE& GetHInstance() { return inst->hInstance; }
	static HWND& GetHWND() { return inst->hwnd; }
};