#pragma once

#ifndef GRAPHICS_CORE
#define GRAPHICS_CORE

#include "Vector3.h"
#include "Vector2.h"

namespace ShaderBuffers
{
	struct ModelConstBuffer
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
	const D3D11_INPUT_ELEMENT_DESC VertexColorShaderInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D11_INPUT_ELEMENT_DESC VertexShaderInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D11_INPUT_ELEMENT_DESC PixelShaderInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}

class Buffer;

namespace GraphicsCore
{
	struct MeshData
	{
		Buffer* vBuffer;
		Buffer* iBuffer;
	};

	struct SimpleVertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexC;

		bool operator<(const SimpleVertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
		};;
	};
}

#endif