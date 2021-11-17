#pragma once
#include "SceneComponent.h"
#include "Mesh.h"

class MeshComponent : public SceneComponent
{
	Mesh* mesh;

protected:
	void Render() override;

public:
	MeshComponent();
	~MeshComponent();
};