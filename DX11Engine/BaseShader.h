#pragma once
#include "GraphicsCore.h"

class Mesh;

using namespace ShaderBuffers;

class BaseShader
{

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	virtual void InitializeShaders();
	virtual void SetShaderParams(ID3D11DeviceContext* deviceContext);
	virtual void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob);
	virtual void CreateRasterizer();
	virtual void CreateStencilState();

	Mesh* mesh;

	ID3D11InputLayout* inputLayout;

	ID3D11RasterizerState* rasterizer;
	ID3D11DepthStencilState* depthStencil;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	Buffer* modelConstBuffer;
	ModelConstBuffer modelCB;

public:
	BaseShader();
	BaseShader(Mesh* mesh);
	virtual ~BaseShader();

	virtual void Render();

	virtual void Initialize(Mesh* mesh);
};
