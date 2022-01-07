#include "CoreMinimal.h"
#include "Graphics.h"

#include "Application.h"
#include "FileImporter.h"
#include "ImGuiManager.h"
#include "PropertyManagerUi.h"

#include "orbit.h"
#include "Mesh.h"
#include "SkyDome.h"
#include "Plane.h"
#include "DirectInput.h"
#include "PostProcessorShader.h"
#include "DemoController.h"

#include "OBJLoader.h"
#include "LitShader.h"
#include "SimplexNoise.h"
#include "Time.h"

Graphics* Graphics::inst = nullptr;

Graphics::~Graphics()
{
    if (deviceContext) deviceContext->ClearState();

    if (deviceContext) deviceContext->Release();
    if (device) device->Release();

    if (swapChain) swapChain->Release();

    if (renderTargetView) renderTargetView->Release();
    if (depthStencilView) depthStencilView->Release();
    if (depthStencilBuffer) depthStencilBuffer->Release();

    if (pixelInputLayout) pixelInputLayout->Release();
    if (vertexInputLayout) vertexInputLayout->Release();

    if (backBuffer) { backBuffer->Release(); }

    delete skyDome;
    delete plane;
    delete sphere;
    delete orbit;
    delete aircraft;

    delete postProcShader;

    delete controller;
}

void Graphics::Initialize(int nCmdShow)
{
    RECT rc;
    GetClientRect(Application::GetHWND(), &rc);
    winWidth = rc.right - rc.left;
    winHeight = rc.bottom - rc.top;

    CreateSwapChain();

	ImGuiManager::Initialize(device, deviceContext);

    CreateDepthStencilView();
    CreateDepthStencilBuff();
    CreateConstBuffers();
    CreateViewport();

    ShowWindow(Application::GetHWND(), nCmdShow);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    //postProcShader = new PostProcessorShader();

    //std::string localFilePath = "";
    //FileImporter::OpenExplorerDialogue(localFilePath);
    sphere = new Mesh(Transform(Vector3(0.f, 0.f, 10.f)), OBJLoader::Load("Assets/Models/Sphere.obj", false), new LitShader(L"", L"Assets/Textures/CrateSpecular.dds", L"Assets/Textures/CrateNormal.dds"));
    plane = new Plane(100, 100, Transform(Vector3(0.f, -5.f, 0.f)), new LitShader());
    orbit = new Orbit(Transform(Vector3(15.f, 0.f, 10.f)));
    aircraft = new Mesh(Transform(Vector3(30.f, 0.f, 10.f), Rotator(), Vector3(0.2f, 0.2f, 0.2f)), OBJLoader::Load("Assets/Models/Hercules.obj", false), new LitShader());
	skyDome = new SkyDome();

    controller = new DemoController();

    DirectInput::BindEngineKeyboardAction(DIK_P, DirectPressed, this, &Graphics::SelectObj); // DEBUG (REMOVE)
}

void Graphics::SelectObj() // DEBUG
{
    PropertyManagerUi* ui = PropertyManagerUi::GetInst();

    ui->SetVisibility(ui->IsVisible() ? false : true);
}

void Graphics::CreateInputLayouts(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(device->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexInputLayout))) // TODO: Replace input layout 
        Quit("Failed to create input layouts");


    vsBlob->Release();
    psBlob->Release();

    deviceContext->IASetInputLayout(vertexInputLayout);
}

void Graphics::CreateDepthStencilView()
{
    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
        Quit(Logger::Fatal, "Back buffer texture failed to be created");

    CD3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = winWidth;
    depthStencilDesc.Height = winHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView)))
        Quit(Logger::Fatal, "Render target view failed to be created");

    //deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);
}

void Graphics::CreateDepthStencilBuff()
{
    CD3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = winWidth;
    depthStencilDesc.Height = winHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer)))
        Quit(Logger::Fatal, "Depth stencil buffer failed to be created");

    if (FAILED(device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView)))
        Quit(Logger::Fatal, "Depth stencil view failed to be created");
}

void Graphics::CreateConstBuffers()
{
    //lightConstBuffer = Buffer::CreateConstBuffer<LightingConstBuffer>();
    //mtrlConstBuffer = Buffer::CreateConstBuffer<MaterialConstBuffer>();
    //
    //lightCB.diffuseLight = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //lightCB.ambientLight = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //lightCB.specularLight = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //lightCB.lightDir = XMFLOAT3(1.f, 1.f, -1.f);
    //
    //mtrlCB.diffuseMtrl = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //mtrlCB.ambientMtrl = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //mtrlCB.specularMtrl = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
    //mtrlCB.specularPower = 10.f;
}

void Graphics::CreateSwapChain()
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
    sd.BufferDesc.Width = winWidth;
    sd.BufferDesc.Height = winHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = Application::GetHWND();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE(driverTypes); driverTypeIndex++)
        if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, (driverType = driverTypes[driverTypeIndex]), nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &deviceContext)))
            break;

    Check(swapChain);
}

void Graphics::CreateViewport()
{
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)winWidth;
    vp.Height = (FLOAT)winHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    deviceContext->RSSetViewports(1, &vp);
}

HRESULT Graphics::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

void Graphics::Render()
{
    float bgColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

    deviceContext->ClearRenderTargetView(renderTargetView, bgColor);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    UINT offset = 0;

    sphere->Render(deviceContext, offset);
    plane->Render(deviceContext, offset);
    skyDome->Render(deviceContext, offset);

    orbit->Update();
    orbit->Render(deviceContext, offset);

    aircraft->SetRotation(aircraft->GetRotation() + Rotator(0.f, Time::GetDeltaTime() * 25.f, 0.f));

    aircraft->Render(deviceContext, offset);

    //----------- debug -----------


    //postProcShader->PreRender(renderTargetView);

    ImGuiManager::GetInst()->Render();

    swapChain->Present(0, 0);
}