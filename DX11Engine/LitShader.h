#pragma once
#include "BaseShader.h"
class LitShader : public BaseShader
{

protected:
	void InitializeShader() override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob) override;
	void SetShaderParams(ID3D11DeviceContext* deviceContext) override;

	Buffer* materialBuffer;
	MaterialConstBuffer materialCB;

	ID3D11ShaderResourceView* diffuseTexture, *specularTexture, *normalTexture;
	ID3D11SamplerState* textureSampler;

	void CreateSampler();

public:
	LitShader(wchar_t* diffusePath = nullptr, wchar_t* specularPath = nullptr, wchar_t* normalPath = nullptr) : BaseShader() { LoadTextures(diffusePath, specularPath, normalPath); }
	LitShader(Mesh* mesh, const wchar_t* diffusePath = nullptr, const wchar_t* specularPath = nullptr, const wchar_t* normalPath = nullptr) : BaseShader() { LoadTextures(diffusePath, specularPath, normalPath); Initialize(mesh); }
	virtual ~LitShader();

	void Initialize(Mesh* mesh) override;

	void LoadTextures(const wchar_t* diffusePath = nullptr, const wchar_t* specularPath = nullptr, const wchar_t* normalPath = nullptr);
	void SetAmbientColor(DirectX::XMFLOAT4 ambientColor) { materialCB.ambientMtrl = ambientColor; }
};