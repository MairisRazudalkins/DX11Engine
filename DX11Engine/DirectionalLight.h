#pragma once
#include "Buffer.h"
#include "Light.h"
#include "GraphicsCore.h"

class Buffer;

using namespace ShaderBuffers;

/// <summary>Only 1 can exist in the scene!</summary>
class DirectionalLight : public Light
{
	static DirectionalLight* inst;

	Vector3 lightDir = Vector3(0.5f, 0.5f, 1.f);

	Buffer* dirLightBuffer;
	DirectionalLightConstBuffer dirLightCB;

	DirectionalLight();
	virtual ~DirectionalLight();

	void UpdateConstBuffers();

public:
	static DirectionalLight* GetInst() { return inst == nullptr ? inst = new DirectionalLight() : inst; }
	ID3D11Buffer* GetDirLightBuffer() { return dirLightBuffer->GetBuffer(); }

	Vector3 GetLightDirection() { return lightDir; }
	void SetLightDirection(Vector3 lightDir) { this->lightDir = lightDir; UpdateConstBuffers(); }

	Color GetDiffuseColor() { return diffuseColor; } // FIX
	void SetDiffuseColor(Color diffuseColor) { this->diffuseColor = diffuseColor; UpdateConstBuffers(); }
};