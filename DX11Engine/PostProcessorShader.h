#pragma once
#include "BaseShader.h"

class PostProcessorShader : public BaseShader
{
	ID3D11SamplerState* textureSampler;

	void CreateSampler();

protected:
	void InitializeShader() override;

public:
	PostProcessorShader();
	~PostProcessorShader();

	void PreRender(ID3D11ShaderResourceView* texture);
};