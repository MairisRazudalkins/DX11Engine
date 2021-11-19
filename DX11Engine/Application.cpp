#include "CoreMinimal.h"
#include "Application.h"
#include "EngineLoop.h"
#include "resource.h"

#include "Input.h"

Application* Application::inst = nullptr;

Application::Application(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance)
{
    inst = this;

    const wchar_t className[] = L"EngineClass";

    RegisterWin(hInstance, className);
	CreateAppWindow(className, L"Engine");
    //CreateRenderTarget(); // fix

    EngineLoop::GetInst()->Initialize(this, nCmdShow);
    EngineLoop::GetInst()->StartEngineLoop();
}

Application::~Application()
{
	delete inst;
}

void Application::RegisterWin(HINSTANCE& hInstance, const wchar_t* className)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = EngineLoop::ProcessWindowsEvent;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = className;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);

    if (!RegisterClassEx(&wcex))
        Logger::ENGINE_LOG(Logger::Fatal, "FAILED TO INIT CLASS");
}

void Application::CreateAppWindow(const wchar_t* className, const wchar_t* windowTitle)
{
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    Check((hwnd = CreateWindow(className, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr)));
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

void Application::Callback(float val)
{
    Logger::ENGINE_LOG(Logger::Info, val);
}