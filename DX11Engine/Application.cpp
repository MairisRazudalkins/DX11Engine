#include "Application.h"
#include "EngineLoop.h"
#include "Input.h"

Application* Application::inst = nullptr;

Application::Application(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance)
{
    const wchar_t className[] = L"EngineClass";

    EngineLoop::GetInst()->Initialize(this, &hInstance, className);

	CreateAppWindow(className, L"Engine");
    CreateSwapChain();

    ShowWindow(hwnd, nCmdShow);

    Input::GetInst()->BindAction(VK_ESCAPE, KeyState::Pressed, this, &Application::Callback);

    device = nullptr;
    Check(device);

    //Input::GetInst()->BindAction(InputDevice::Keyboard, 0x04, KeyState::Pressed, this, &Application::Callback);

    EngineLoop::GetInst()->StartEngineLoop();
}

Application::~Application()
{
	if (deviceContext) deviceContext->ClearState();

	if (deviceContext) deviceContext->Release();
	if (renderTarget) renderTarget->Release();
	if (swapChain) swapChain->Release();
	if (device) device->Release();

	delete inst;
}

void Application::CreateAppWindow(const wchar_t* className, const wchar_t* windowTitle)
{
    Check((hwnd = CreateWindowEx(0, className, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL)));
}

void Application::CreateSwapChain()
{
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
    D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = wWidth;
    sd.BufferDesc.Height = wHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE(driverTypes); driverTypeIndex++)
        if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, (driverType = driverTypes[driverTypeIndex]), nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &deviceContext)))
            break;

    Check(swapChain);
}

void Application::Callback()
{
    PostQuitMessage(0);
}

void Application::Update()
{
	
}

void Application::Render()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    EndPaint(hwnd, &ps);
}