#include "CoreMinimal.h"
#include "LitShader.h"

#include "Camera.h"
#include "DDSTextureLoader.h"
#include "DirectionalLight.h"
#include "Graphics.h"

LitShader::~LitShader()
{
	delete materialBuffer;

	if (diffuseTexture) { diffuseTexture->Release(); }
	if (specularTexture) { specularTexture->Release(); }
	if (normalTexture) { normalTexture->Release(); }
	if (textureSampler) { textureSampler->Release(); }
}

void LitShader::InitializeShader()
{
	ID3D11Device* device = Graphics::GetDevice();
	ID3DBlob* vsBlob = nullptr, * psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"LitShader.fx", "VS_LIT", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"LitShader.fx", "PS_LIT", "ps_4_0", &psBlob))) // compile shader
		Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
		Quit("Failed to compile shader from file");

	InitializeInputLayout(vsBlob, psBlob);
	CreateSampler();
}

void LitShader::InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))) // TODO: Replace input layout 
		Quit("Failed to create input layouts");

	vsBlob->Release();
	psBlob->Release();
}

void LitShader::CreateSampler()
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


void LitShader::SetShaderParams(ID3D11DeviceContext* deviceContext)
{
	BaseShader::SetShaderParams(deviceContext);

	ID3D11Buffer* buffer = DirectionalLight::GetInst()->GetDirLightBuffer(); // directional light buffer
	deviceContext->VSSetConstantBuffers(1, 1, &buffer);
	deviceContext->PSSetConstantBuffers(1, 1, &buffer);

	materialCB.eyePos = Camera::GetActiveCamera()->GetPosition().ToFloat3();

	ID3D11Buffer* matBuffer = materialBuffer->GetBuffer();
	deviceContext->UpdateSubresource(matBuffer, 0, nullptr, &materialCB, 0, 0); // material buffer
	deviceContext->VSSetConstantBuffers(2, 1, &matBuffer);
	deviceContext->PSSetConstantBuffers(2, 1, &matBuffer);

	deviceContext->PSSetSamplers(0, 1, &textureSampler);
	deviceContext->PSSetShaderResources(0, 1, &diffuseTexture);
	deviceContext->PSSetShaderResources(1, 1, &specularTexture);
	deviceContext->PSSetShaderResources(2, 1, &normalTexture);
}

void LitShader::Initialize(Mesh* mesh)
{
	materialBuffer = Buffer::CreateConstBuffer<MaterialConstBuffer>();

	materialCB.diffuseMtrl = diffuseTexture == nullptr ? DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	materialCB.ambientMtrl = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	materialCB.specularMtrl = specularTexture == nullptr ? DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	materialCB.specularPower = 1.0f;

	BaseShader::Initialize(mesh);
}

void LitShader::LoadTextures(const wchar_t* diffusePath, const wchar_t* specularPath, const wchar_t* normalPath)
{
	if (diffusePath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), diffusePath, nullptr, &diffuseTexture);

	if (specularPath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), specularPath, nullptr, &specularTexture);

	if (normalPath != nullptr)
		DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), normalPath, nullptr, &normalTexture);
}