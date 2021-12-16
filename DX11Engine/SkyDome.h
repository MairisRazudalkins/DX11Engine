#pragma once
#include "Mesh.h"

class SkyDomeShader;

class SkyDome : public Mesh
{

public:
	SkyDome();
	~SkyDome();

	//DirectX::XMFLOAT4X4 GetWorldMatrix() override;
	virtual DirectX::XMMATRIX GetMatrix() override;
};
