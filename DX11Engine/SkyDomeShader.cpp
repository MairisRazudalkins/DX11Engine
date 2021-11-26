#include "CoreMinimal.h"
#include "SkyDomeShader.h"

#include "Camera.h"
#include "DDSTextureLoader.h"
#include "SkyDome.h"
#include "Graphics.h"

SkyDomeShader::SkyDomeShader(SkyDome* skyDome)
{
	this->skyDome = skyDome;

	modelConstBuffer = Buffer::CreateConstBuffer<ModelConstBuffer>();

	HRESULT hr = DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), L"Assets/Textures/SkySphere.dds", nullptr, &resourceView); // TODO: FIX THIS :D
	
	if (!FAILED(hr))
	{
		Graphics::GetDeviceContext()->PSSetShaderResources(0, 1, &resourceView);
	
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
		Graphics::GetDevice()->CreateSamplerState(&sampDesc, &linearSampler);
	}
	
	Graphics::GetDeviceContext()->PSSetSamplers(0, 1, &linearSampler);

	InitializeShaders();
	CreateRasterizerState();
	CreateTextures();
}

SkyDomeShader::~SkyDomeShader()
{
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();

	if (pixelInputLayout) pixelInputLayout->Release();
	if (vertexInputLayout) vertexInputLayout->Release();

	if (resourceView) resourceView->Release();

	skyDome = nullptr;

	delete modelConstBuffer;
}

void SkyDomeShader::Render()
{
	ID3D11DeviceContext* deviceContext = Graphics::GetDeviceContext();

	SetShaderParams(deviceContext);

	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
}

void SkyDomeShader::InitializeShaders()
{
	ID3D11Device* device = Graphics::GetDevice();
	ID3DBlob* vsBlob = nullptr, *psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"SkyDomeShader.fx", "VS", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"SkyDomeShader.fx", "PS", "ps_4_0", &psBlob))) // compile shader
		Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
		Quit("Failed to compile shader from file");

	InitializeInputLayout(vsBlob, psBlob);
}

void SkyDomeShader::InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexInputLayout))) // TODO: Replace input layout 
		Quit("Failed to create input layouts");

	//if (FAILED(device->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), psBlob->GetBufferPointer(), psBlob->GetBufferSize(), &pixelInputLayout))) // TODO: Replace input layout 
	//    Quit("Failed to create input layouts");

	vsBlob->Release();
	psBlob->Release();

	Graphics::GetDeviceContext()->IASetInputLayout(vertexInputLayout);
}

void SkyDomeShader::SetShaderParams(ID3D11DeviceContext* deviceContext)
{
	if (skyDome)
	{
		modelCB = Graphics::GetModelCB();
		ID3D11Buffer* modelBuffer = modelConstBuffer->GetBuffer();

		//modelCB.world = Camera::GetActiveCamera()->GetMatrix();
		modelCB.world = skyDome->GetMatrix();

		deviceContext->RSSetState(rasterizerState);

		deviceContext->UpdateSubresource(modelBuffer, 0, nullptr, &modelCB, 0, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &modelBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &modelBuffer);
	}
}

void SkyDomeShader::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = true;

	Graphics::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	Graphics::GetDeviceContext()->RSSetState(rasterizerState);
}

void SkyDomeShader::CreateTextures()
{

}