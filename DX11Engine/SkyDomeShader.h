#pragma once
#include "BaseShader.h"

class SkyDome;

class SkyDomeShader : public BaseShader
{
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* linearSampler;

	Buffer* colorBuffer;
	SkyDomeColorConstBuffer colorCB;

	void CreateTextures();

protected:
	void InitializeShader() override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob) override;
	void CreateRasterizer() override;
	void SetShaderParams(ID3D11DeviceContext* deviceContext) override;

public:
	SkyDomeShader();
	SkyDomeShader(SkyDome* skyDome);
	virtual ~SkyDomeShader();

	void LerpSkyDomeColor(float val);
};