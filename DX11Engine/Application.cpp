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
    //CreateRenderTarget(); // fix

    ShowWindow(hwnd, nCmdShow);

    Input::GetInst()->BindAction(VK_ESCAPE, KeyState::Pressed, this, &Application::Callback); // DEBUG REMOVE!

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

void Application::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (!hr)
    {
        Logger::ENGINE_LOG(Logger::Error, "Failed to create back buffer")
    }

    hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTarget);

    if (!hr)
    {
        Logger::ENGINE_LOG(Logger::Error, "Failed to create render target")
    }

    pBackBuffer->Release();

    //ID3D11Texture2D* backBuffer = nullptr;
    //swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    //
    //Check((device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget)));
    //backBuffer->Release();
}

void Application::InitializeShaderInputLayouts()
{
    //ID3DBlob* pVSBlob = nullptr;
    //HRESULT hr = CompileShaderFromFile(L"Test", "VS", "vs_4_0", &pVSBlob);
    //
    //
}

HRESULT Application::CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) 
            pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

void Application::Callback()
{
    PostQuitMessage(0);
}

void Application::Update()
{
	Logger::ENGINE_LOG(Logger::Info, "Update Called")
}

void Application::Render()
{
    //Logger::ENGINE_LOG(Logger::Info, "Rendering");
    //float bgColor[4]{ 0.f, 0.f, 0.f, 0.f }; // TODO: Replace with color class
    //deviceContext->ClearRenderTargetView(renderTarget, bgColor);


    swapChain->Present(0, 0);
}