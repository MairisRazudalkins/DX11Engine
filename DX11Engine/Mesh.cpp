#include "CoreMinimal.h"
#include "GraphicsCore.h"
#include "Mesh.h"

#include "DDSTextureLoader.h"

Mesh::Mesh(int vCount, GraphicsCore::SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs) : Object()
{
	LoadTexture();
	vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(verts, vCount, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<int>(triangles, tCount, BufferBindFlag::Index);
}

Mesh::Mesh(Buffer* vBuffer, Buffer* iBuffer) : Object()
{
	LoadTexture();
	this->vBuffer = vBuffer;
	this->iBuffer = iBuffer;
}

Mesh::Mesh(GraphicsCore::MeshData data)
{
	LoadTexture();
	this->vBuffer = data.vBuffer;
	this->iBuffer = data.iBuffer;
}

Mesh::~Mesh()
{
	delete vBuffer;
	delete iBuffer;
}

void Mesh::LoadTexture()
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