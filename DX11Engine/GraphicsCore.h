#pragma once
#include "Vector3.h"
#include "Vector2.h"

struct SimpleVertex
{
	Vector3 vPos;
	Vector3 vNorm;
	Vector2 texCoord;

	SimpleVertex(Vector3 vPos, Vector3 vNorm, Vector2 texCoord)
	{
		this->vPos = vPos;
		this->vNorm = vNorm;
		this->texCoord = texCoord;
	}
};

namespace ShaderBuffers
{
	struct WorldBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct LightingBuffer // TODO: Implement
	{
		
	};

	struct MaterialBuffer // TODO: Implement
	{
		
	};
}

namespace InputLayout
{
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
}

