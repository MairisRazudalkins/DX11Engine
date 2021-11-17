#pragma once
#include "Object.h"
#include "Buffer.h"
#include "GraphicsCore.h"

class Mesh : public Object
{

public:
	Buffer* vBuffer;
	Buffer* iBuffer;

	Mesh() : Object() {};
	Mesh(int vCount, VertexData::SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs);
	Mesh(Buffer* vBuffer, Buffer* iBuffer);

	virtual ~Mesh();
};