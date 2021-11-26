#pragma once
#include "GraphicsCore.h"

using namespace ShaderBuffers;

class BaseShader
{

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	virtual void InitializeShaders() = 0;
	virtual void SetShaderParams(ID3D11DeviceContext* deviceContext) = 0;

	ID3D11InputLayout* inputLayout;

public:
	BaseShader();
	virtual ~BaseShader();

	virtual void Render() = 0;
};
