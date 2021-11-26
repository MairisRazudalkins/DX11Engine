#include "CoreMinimal.h"
#include "GraphicsCore.h"
#include "Mesh.h"

#include "BaseShader.h"
#include "DDSTextureLoader.h"

Mesh::Mesh(Transform transform, int vCount, GraphicsCore::SimpleVertex* verts, int tCount, int* triangles, BaseShader* shader) : SceneObject(transform)
{
	vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(verts, vCount, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<int>(triangles, tCount, BufferBindFlag::Index);

	this->shader = shader;
	this->shader->Initialize(this);
}

Mesh::Mesh(Transform transform, Buffer* vBuffer, Buffer* iBuffer, BaseShader* shader) : SceneObject(transform)
{
	this->vBuffer = vBuffer;
	this->iBuffer = iBuffer;

	this->shader = shader;
	this->shader->Initialize(this);
}

Mesh::Mesh(Transform transform, GraphicsCore::MeshData data, BaseShader* shader) : SceneObject(transform)
{
	this->vBuffer = data.vBuffer;
	this->iBuffer = data.iBuffer;

	this->shader = shader;
	this->shader->Initialize(this);
}

Mesh::~Mesh()
{
	delete vBuffer;
	delete iBuffer;
	delete shader;
}

void Mesh::Render(ID3D11DeviceContext* deviceContext, UINT& offset)
{
	ID3D11Buffer* vBuffer = this->vBuffer->GetBuffer(), *iBuffer = this->iBuffer->GetBuffer();
	UINT stride = sizeof(GraphicsCore::SimpleVertex);

	if (shader)
		shader->Render();

	deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(this->iBuffer->GetBufferSize(), 0, 0);
}

void Mesh::LoadTexture() // DEBUG
{
	HRESULT hr = DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), L"Assets/Textures/Crate_COLOR.dds", nullptr, &textureResourceView);
	
	if (!FAILED(hr))
	{
		Graphics::GetDeviceContext()->PSSetShaderResources(0, 1, &textureResourceView);
	
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
	
	hr = DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), L"Assets/Textures/Crate_SPEC.dds", nullptr, &textureResourceView);
	
	if (!FAILED(hr))
	{
		Graphics::GetDeviceContext()->PSSetShaderResources(1, 1, &textureResourceView);
	
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
	
	hr = DirectX::CreateDDSTextureFromFile(Graphics::GetDevice(), L"Assets/Textures/Crate_NRM.dds", nullptr, &textureResourceView);
	
	if (!FAILED(hr))
	{
		Graphics::GetDeviceContext()->PSSetShaderResources(2, 1, &textureResourceView);
	
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
}