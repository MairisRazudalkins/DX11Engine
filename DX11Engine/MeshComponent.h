#pragma once
#include "SceneComponent.h"
#include "Mesh.h"

class MeshComponent : public SceneComponent
{
	Mesh* mesh;

public:
	MeshComponent();
	~MeshComponent();

	void Render() override;
};