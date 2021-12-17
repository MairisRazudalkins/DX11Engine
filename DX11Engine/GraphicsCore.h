#pragma once

#ifndef GRAPHICS_CORE
#define GRAPHICS_CORE

namespace ShaderBuffers
{
	// __declspec(align(16)) <--- fix dx11 buffer complaining that the data isn't a multiple of 16

	struct ModelConstBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	__declspec(align(16))
	struct DirectionalLightConstBuffer
	{
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 specularColor;
		DirectX::XMFLOAT3 lightDirection;
	};

	__declspec(align(16))
	struct LightingConstBuffer // TODO: Implement
	{
		DirectX::XMFLOAT4 diffuseLight;
		DirectX::XMFLOAT4 ambientLight;
		DirectX::XMFLOAT4 specularLight;
		DirectX::XMFLOAT3 lightDir;
	};

	__declspec(align(16))
	struct MaterialConstBuffer // TODO: Implement
	{
		DirectX::XMFLOAT4 diffuseMtrl;
		DirectX::XMFLOAT4 ambientMtrl;
		DirectX::XMFLOAT4 specularMtrl;
		float specularPower;
		DirectX::XMFLOAT3 eyePos;
	};

	__declspec(align(16))
	struct SkyDomeColorConstBuffer // TODO: Implement
	{
		DirectX::XMFLOAT4 color1;
		DirectX::XMFLOAT4 color2;
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
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoord;

		bool operator<(const SimpleVertex other) const
		{
			return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
		};
	};
}

#endif