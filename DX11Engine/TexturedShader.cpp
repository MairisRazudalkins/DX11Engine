#include "CoreMinimal.h"
#include "TexturedShader.h"

#include "DDSTextureLoader.h"
#include "Graphics.h"

TexturedShader::TexturedShader(wchar_t* diffusePath, wchar_t* specularPath, wchar_t* normalPath) : BaseShader()
{
	LoadTextures(diffusePath, specularPath, normalPath);
	InitializeShader();
}

TexturedShader::~TexturedShader()
{
	if (diffuseTexture) { diffuseTexture->Release(); }
	if (specularTexture) { specularTexture->Release(); }
	if (normalTexture) { normalTexture->Release(); }
	if (textureSampler) { textureSampler->Release(); }
}

void TexturedShader::LoadTextures(const wchar_t* diffusePath, const wchar_t* specularPath, const wchar_t* normalPath)
{
	if (diffusePath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), diffusePath, nullptr, &diffuseTexture);

	if (specularPath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), specularPath, nullptr, &specularTexture);

	if (normalPath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), normalPath, nullptr, &normalTexture);
}

void TexturedShader::InitializeShader()
{
	ID3D11Device* device = Graphics::GetDevice();
	ID3DBlob* vsBlob = nullptr, * psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"LitShader.fx", "VS_LIT", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"TexturedShader.fx", "PS_TEXTURED", "ps_4_0", &psBlob))) // compile shader
		Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
		Quit("Failed to compile shader from file");

	InitializeInputLayout(vsBlob, psBlob);
	CreateSampler();
}

void TexturedShader::InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))) // TODO: Replace input layout 
		Quit("Failed to create input layouts");

	vsBlob->Release();
	psBlob->Release();
}

void TexturedShader::SetShaderParams(ID3D11DeviceContext* deviceContext)
{
	BaseShader::SetShaderParams(deviceContext);

	deviceContext->PSSetSamplers(0, 1, &textureSampler);
	deviceContext->PSSetShaderResources(0, 1, &diffuseTexture);
	deviceContext->PSSetShaderResources(1, 1, &specularTexture);
	deviceContext->PSSetShaderResources(2, 1, &normalTexture);
}

void TexturedShader::CreateSampler()
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
