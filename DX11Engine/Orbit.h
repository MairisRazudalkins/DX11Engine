#pragma once
#include "Mesh.h"
class Orbit :public Mesh
{
#define PLANET_COUNT 10

	Mesh* meshes[PLANET_COUNT];
	Rotator rotations[PLANET_COUNT];

public:
	Orbit(Transform transform);
	virtual ~Orbit();

	virtual void Render(ID3D11DeviceContext* deviceContext, UINT& offset) override;
	virtual void Update() override;
};