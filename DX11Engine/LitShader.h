#pragma once
#include "BaseShader.h"
class LitShader : public BaseShader
{

protected:
	void InitializeShaders() override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob) override;
	void SetShaderParams(ID3D11DeviceContext* deviceContext) override;

	Buffer* materialBuffer;
	MaterialConstBuffer materialCB;

public:
	LitShader() : BaseShader() {};

	void Initialize(Mesh* mesh) override;
};