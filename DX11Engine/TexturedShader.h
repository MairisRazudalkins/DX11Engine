#pragma once
#include "BaseShader.h"
class TexturedShader :public BaseShader
{
	void InitializeShader() override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob) override;
	void SetShaderParams(ID3D11DeviceContext* deviceContext) override;

	void CreateSampler();

	ID3D11ShaderResourceView* diffuseTexture, * specularTexture, * normalTexture;
	ID3D11SamplerState* textureSampler;

public:
	TexturedShader(wchar_t* diffusePath = nullptr, wchar_t* specularPath = nullptr, wchar_t* normalPath = nullptr);
	virtual ~TexturedShader();

	void LoadTextures(const wchar_t* diffusePath = nullptr, const wchar_t* specularPath = nullptr, const wchar_t* normalPath = nullptr);
};