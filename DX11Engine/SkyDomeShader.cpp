#include "CoreMinimal.h"
#include "SkyDomeShader.h"

#include "Camera.h"
#include "DDSTextureLoader.h"
#include "SkyDome.h"
#include "Graphics.h"

SkyDomeShader::SkyDomeShader()
{
}

SkyDomeShader::SkyDomeShader(SkyDome* skyDome) : BaseShader(skyDome)
{
	modelConstBuffer = Buffer::CreateConstBuffer<ModelConstBuffer>();

	InitializeShader();
	CreateRasterizer();
	CreateTextures();
}

SkyDomeShader::~SkyDomeShader()
{
	if (resourceView) resourceView->Release();
}

void SkyDomeShader::InitializeShader()
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
	if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))) // TODO: Replace input layout 
		Quit("Failed to create input layouts");

	vsBlob->Release();
	psBlob->Release();
}

void SkyDomeShader::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.FrontCounterClockwise = false;

	Graphics::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizer);
}

void SkyDomeShader::CreateTextures()
{
	if (!FAILED(DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), L"Assets/Textures/SkySphere.dds", nullptr, &resourceView)))
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
}