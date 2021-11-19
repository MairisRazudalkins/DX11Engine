#include "CoreMinimal.h"
#include "Graphics.h"

#include "Application.h"
#include "Camera.h"

#include "Mesh.h"
#include "OBJLoader.h"

Graphics* Graphics::inst = nullptr;
int Graphics::winWidth = 500;
int Graphics::winHeight = 500;

Graphics::~Graphics()
{
    if (deviceContext) deviceContext->ClearState();

    if (deviceContext) deviceContext->Release();
    if (device) device->Release();

    if (swapChain) swapChain->Release();

    if (renderTargetView) renderTargetView->Release();
    if (depthStencilView) depthStencilView->Release();

    if (vertexShader) vertexShader->Release();
    if (pixelShader) pixelShader->Release();

    if (pixelInputLayout) pixelInputLayout->Release();
    if (vertexInputLayout) vertexInputLayout->Release();
}

void Graphics::Initialize(int nCmdShow)
{
    RECT rc;
    GetClientRect(Application::GetHWND(), &rc);
    winWidth = rc.right - rc.left;
    winHeight = rc.bottom - rc.top;

    CreateSwapChain();
    InitializeShaders();

    CreateDepthStencilView();
    CreateDepthStencilBuff();
    CreateConstBuffer();
    CreateViewport();

    ShowWindow(Application::GetHWND(), nCmdShow);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    //debug

    XMStoreFloat4x4(&_world, DirectX::XMMatrixIdentity());

    DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 10.0f, -10.0f, 0.0f);
    DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMStoreFloat4x4(&_view, DirectX::XMMatrixLookAtLH(Eye, At, Up));
    XMStoreFloat4x4(&_projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, winWidth / (FLOAT)winHeight, 0.01f, 100.0f));

    //GraphicsCore::SimpleVertex verts[] =
    //{
    //    {DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //    {DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //
    //    {DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //    {DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //
    //    {DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //    {DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //
    //    {DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //    {DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f) },
    //};
    //
    //WORD indices[] = { 0,1,2, 2,1,3, 3,1,4, 4,5,3, 5,4,6, 7,5,6, 7,0,2, 7,6,0, 4,1,0, 0,6,4, 3,5,2, 5,7,2 };

    //Buffer* vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(verts, 8, BufferBindFlag::Vertex);
    //Buffer* iBuffer = Buffer::CreateBuffer<WORD>(indices, 36, BufferBindFlag::Index);

    GraphicsCore::MeshData data = OBJLoader::Load("Assets/Models/Cube.obj", device, false);

    mesh = new Mesh(data);
}

void Graphics::InitializeShaders()
{
	ID3DBlob* vsBlob = nullptr, *psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &psBlob))) // compile shader
        Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
        Quit("Failed to compile shader from file");

    CreateInputLayouts(vsBlob, psBlob);
}

void Graphics::CreateInputLayouts(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(device->CreateInputLayout(InputLayout::VertexShaderInputLayout, ARRAYSIZE(InputLayout::VertexShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexInputLayout))) // TODO: Replace input layout 
        Quit("Failed to create input layouts");

    //if (FAILED(device->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), psBlob->GetBufferPointer(), psBlob->GetBufferSize(), &pixelInputLayout))) // TODO: Replace input layout 
    //    Quit("Failed to create input layouts");

    vsBlob->Release();
    psBlob->Release();

    deviceContext->IASetInputLayout(vertexInputLayout);
}

void Graphics::CreateDepthStencilView()
{
    ID3D11Texture2D* backBuff = nullptr;

    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuff)))
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

    if (FAILED(device->CreateRenderTargetView(backBuff, nullptr, &renderTargetView)))
        Quit(Logger::Fatal, "Render target view failed to be created");

    backBuff->Release();
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

void Graphics::CreateConstBuffer()
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ShaderBuffers::ModelConstBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    if (FAILED(device->CreateBuffer(&bd, nullptr, &_pConstantBuffer)))
        Quit(Logger::Fatal, "Failed to create constant buffer")
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

    //----------- debug -----------

    DirectX::XMMATRIX viewMatrix = Camera::GetActiveCamera()->GetMatrix();

    DirectX::XMMATRIX world = XMLoadFloat4x4(&_world);
    //DirectX::XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
    DirectX::XMMATRIX projection = XMLoadFloat4x4(&_projection);

    cb.world = DirectX::XMMatrixTranspose(world);
    cb.projection = DirectX::XMMatrixTranspose(projection);
    cb.view = DirectX::XMMatrixTranspose(viewMatrix);

    deviceContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);

    deviceContext->PSSetShader(pixelShader, nullptr, 0);
    deviceContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);

    UINT stride = sizeof(GraphicsCore::SimpleVertex), offset = 0;

    ID3D11Buffer* vBuffer = this->mesh->vBuffer->GetBuffer();
    ID3D11Buffer* iBuffer = this->mesh->iBuffer->GetBuffer();

    // TODO: Assign vShader and pShader

    deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R16_UINT, 0);
    deviceContext->DrawIndexed(this->mesh->iBuffer->GetBufferSize(), 0, 0);

    //----------- debug -----------

    swapChain->Present(0, 0);
}