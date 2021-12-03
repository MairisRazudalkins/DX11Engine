#pragma once
#include "BaseShader.h"

class SkyDome;

class SkyDomeShader : public BaseShader
{
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* linearSampler;
;
	void CreateTextures();

protected:
	void InitializeShaders() override;
	void InitializeInputLayout(ID3DBlob* vsBlob, ID3DBlob* psBlob) override;
	void CreateRasterizer() override;

public:
	SkyDomeShader();
	SkyDomeShader(SkyDome* skyDome);
	virtual ~SkyDomeShader();
};