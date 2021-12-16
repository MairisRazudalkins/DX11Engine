#include "CoreMinimal.h"
#include "PostProcessorShader.h"

#include "Graphics.h"


PostProcessorShader::PostProcessorShader()
{
	InitializeShader();
}

PostProcessorShader::~PostProcessorShader()
{
	if (textureSampler) { textureSampler->Release(); }
}

void PostProcessorShader::PreRender(ID3D11ShaderResourceView* texture)
{
	Graphics::GetDeviceContext()->PSSetSamplers(0, 1, &textureSampler);
	Graphics::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
}

void PostProcessorShader::CreateSampler()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(Graphics::GetDevice()->CreateSamplerState(&sampDesc, &textureSampler)))
		Logger::ENGINE_LOG(Logger::Info, "Failed to create texture sampler");
}

void PostProcessorShader::InitializeShader()
{
	CreateSampler();

	ID3D11Device* device = Graphics::GetDevice();
	ID3DBlob* vsBlob = nullptr, * psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"SkyDomeShader.fx", "VS", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"SkyDomeShader.fx", "PS", "ps_4_0", &psBlob))) // compile shader
		Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
		Quit("Failed to compile shader from file");

	InitializeInputLayout(vsBlob, psBlob);
}