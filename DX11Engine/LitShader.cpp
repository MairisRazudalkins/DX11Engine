#include "CoreMinimal.h"
#include "LitShader.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "Graphics.h"

void LitShader::InitializeShaders()
{
	ID3D11Device* device = Graphics::GetDevice();
	ID3DBlob* vsBlob = nullptr, * psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"LitShader.fx", "VS_LIT", "vs_4_0", &vsBlob)) || FAILED(CompileShaderFromFile(L"LitShader.fx", "PS_LIT", "ps_4_0", &psBlob))) // compile shader
		Quit("Failed to compile shader from file");

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader)) || FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) // create shader
		Quit("Failed to compile shader from file");

	InitializeInputLayout(vsBlob, psBlob);
}

void LitShader::InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob)
{
	if (FAILED(Graphics::GetDevice()->CreateInputLayout(InputLayout::PixelShaderInputLayout, ARRAYSIZE(InputLayout::PixelShaderInputLayout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout))) // TODO: Replace input layout 
		Quit("Failed to create input layouts");

	vsBlob->Release();
	psBlob->Release();
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
}

void LitShader::Initialize(Mesh* mesh)
{
	materialBuffer = Buffer::CreateConstBuffer<MaterialConstBuffer>();

	materialCB.diffuseMtrl = DirectX::XMFLOAT4(1.f, 0.2f, 0.2f, 1.0f);
	materialCB.ambientMtrl = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	materialCB.specularMtrl = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	materialCB.shininess = DirectX::XMFLOAT4(5.0f, 5.0f, 5.0f, 5.0f);

	BaseShader::Initialize(mesh);
}
