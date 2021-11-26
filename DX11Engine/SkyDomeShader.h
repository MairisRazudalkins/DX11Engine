#pragma once
#include "BaseShader.h"

class SkyDome;

class SkyDomeShader : public BaseShader
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* linearSampler;

	ID3D11RasterizerState* rasterizerState;

	ID3D11InputLayout* pixelInputLayout;
	ID3D11InputLayout* vertexInputLayout;

	Buffer* modelConstBuffer;
	ModelConstBuffer modelCB;

	SkyDome* skyDome;

	void CreateTextures();
	void CreateRasterizerState();

protected:
	void InitializeShaders() override;
	void SetShaderParams(ID3D11DeviceContext* deviceContext) override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob);

public:
	SkyDomeShader(SkyDome* skyDome);
	virtual ~SkyDomeShader();

	void Render() override;
};