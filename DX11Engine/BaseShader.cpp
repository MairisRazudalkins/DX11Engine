#include "CoreMinimal.h"
#include "BaseShader.h"
#include "GraphicsCore.h"
#include "Graphics.h"
#include "Buffer.h"
#include "Camera.h"
#include "Mesh.h"

// TODO: Create a basic vertex shader which can be used by any class

ID3D11RasterizerState* BaseShader::globalRasterizer = nullptr;

BaseShader::BaseShader()
{

}

BaseShader::BaseShader(Mesh* mesh) : mesh(mesh)
{
    //modelConstBuffer = Buffer::CreateConstBuffer<ShaderBuffers::ModelConstBuffer>();
    //
    //InitializeShaders();
    //CreateRasterizer();
}

void BaseShader::Initialize(Mesh* mesh)
{
    this->mesh = mesh;

    modelConstBuffer = Buffer::CreateConstBuffer<ShaderBuffers::ModelConstBuffer>();

    InitializeShader();
    CreateRasterizer();
}

void BaseShader::SetGlobalRasterizer(D3D11_FILL_MODE state)
{
	if (state == D3D11_FILL_SOLID)
	{
		if (globalRasterizer) { globalRasterizer->Release(); }

        globalRasterizer = nullptr;
        return;
	}

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = state;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;

    Graphics::GetDevice()->CreateRasterizerState(&rasterizerDesc, &globalRasterizer);
}

BaseShader::~BaseShader()
{
    if (inputLayout) inputLayout->Release();

    if (rasterizer) rasterizer->Release();
    if (depthStencil) depthStencil->Release();
        
    if (vertexShader) vertexShader->Release();
    if (pixelShader) pixelShader->Release();

    delete modelConstBuffer; 
}

void BaseShader::Render()
{
    ID3D11DeviceContext* deviceContext = Graphics::GetDeviceContext();

    deviceContext->RSSetState(globalRasterizer == nullptr ? rasterizer : globalRasterizer);
    deviceContext->IASetInputLayout(inputLayout);

    SetShaderParams(deviceContext);

    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);
}

HRESULT BaseShader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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

void BaseShader::InitializeShader()
{
    ID3D11Device* device = Graphics::GetDevice();
    ID3DBlob* vsBlob = nullptr, * psBlob = nullptr;

    if (FAILED(CompileShaderFromFile(L"UnlitShader.fx", "VS_UNLIT", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"UnlitShader.fx", "PS_UNLIT", "ps_4_0", &psBlob))) // compile shader
        Quit("Failed to compile shader from file");

    if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
        Quit("Failed to compile shader from file");

    InitializeInputLayout(vsBlob, psBlob);
}

void BaseShader::SetShaderParams(ID3D11DeviceContext* deviceContext)
{
    if (mesh)
    {
        modelCB.world = DirectX::XMMatrixTranspose(mesh->GetMatrix());
        //modelCB.world = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&mesh->GetWorldMatrix()));
        modelCB.projection = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&Camera::GetActiveCamera()->GetProjection()));
        modelCB.view = DirectX::XMMatrixTranspose(Camera::GetActiveCamera()->GetCameraMatrix());

        ID3D11Buffer* modelBuffer = modelConstBuffer->GetBuffer();

        deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &modelCB, 0, 0);

        deviceContext->VSSetConstantBuffers(0, 1, &modelBuffer);
        deviceContext->PSSetConstantBuffers(0, 1, &modelBuffer);
    }
}

void BaseShader::InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
    if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))) // TODO: Replace input layout 
        Quit("Failed to create input layouts");

    vsBlob->Release();
    psBlob->Release();
}

void BaseShader::CreateRasterizer()
{
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;

    Graphics::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizer);
}

void BaseShader::CreateStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilStateDesc.DepthEnable = true;
}