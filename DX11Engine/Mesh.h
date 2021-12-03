#pragma once
#include "SceneObject.h"
#include "Buffer.h"

class BaseShader;

class Mesh : public SceneObject
{

public:
	Buffer* vBuffer;
	Buffer* iBuffer;

	ID3D11ShaderResourceView* textureResourceView = nullptr; // remove
	ID3D11SamplerState* linearSampler = nullptr;

	BaseShader* shader;

	Mesh(Transform transform, BaseShader* shader): SceneObject(transform) { vBuffer = nullptr; iBuffer = nullptr; this->shader = shader; }
	Mesh(Transform transform, int vCount, GraphicsCore::SimpleVertex* verts, int tCount, int* triangles, BaseShader* shader);
	Mesh(Transform transform, Buffer* vBuffer, Buffer* iBuffer, BaseShader* shader);
	Mesh(Transform transform, GraphicsCore::MeshData data, BaseShader* shader);

	virtual void Render(ID3D11DeviceContext* deviceContext, UINT& offset);

	virtual ~Mesh();

	void LoadTexture();
};