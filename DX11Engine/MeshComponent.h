#pragma once
#include "SceneComponent.h"
#include "Mesh.h"

class MeshComponent : public SceneComponent
{
	Mesh* mesh;

	DirectX::XMFLOAT4X4 worldMatrix;


protected:
	void Render() override;

public:
	MeshComponent();
	~MeshComponent();
};