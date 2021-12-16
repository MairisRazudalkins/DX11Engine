#pragma once
#include "Mesh.h"
class Plane : public Mesh
{
	void AddTriangles(WORD* indices, int& triangleIndex, int a, int b, int c);
	void CalculateNormals(GraphicsCore::SimpleVertex* verts, int count);
	float* LoadHeightmap(int width, int height, float scale);

public:
	Plane(const int numX, const int numY, Transform transform, BaseShader* shader);
	virtual ~Plane() = default;
};