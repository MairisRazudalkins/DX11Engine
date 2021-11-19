#pragma once
#include "Object.h"
#include "Buffer.h"

class Mesh : public Object
{

public:
	Buffer* vBuffer;
	Buffer* iBuffer;

	Mesh() : Object() { vBuffer = nullptr; iBuffer = nullptr; }
	Mesh(int vCount, GraphicsCore::SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs);
	Mesh(Buffer* vBuffer, Buffer* iBuffer);
	Mesh(GraphicsCore::MeshData data);

	virtual ~Mesh();
};