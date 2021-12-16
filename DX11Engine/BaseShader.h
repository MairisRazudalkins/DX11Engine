#pragma once
#include "GraphicsCore.h"

class Mesh;

using namespace ShaderBuffers;

class BaseShader
{

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	virtual void InitializeShader();
	virtual void SetShaderParams(ID3D11DeviceContext* deviceContext);
	virtual void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob);
	virtual void CreateRasterizer();
	virtual void CreateStencilState();

	Mesh* mesh;

	ID3D11InputLayout* inputLayout;

	static ID3D11RasterizerState* globalRasterizer;
	ID3D11RasterizerState* rasterizer;
	ID3D11DepthStencilState* depthStencil;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	DirectX::XMFLOAT4X4 world;

	Buffer* modelConstBuffer;
	ModelConstBuffer modelCB;

public:
	BaseShader();
	BaseShader(Mesh* mesh);
	virtual ~BaseShader();

	virtual void Render();
	virtual void Initialize(Mesh* mesh);

	static void SetGlobalRasterizer(D3D11_FILL_MODE fillMode);
};
