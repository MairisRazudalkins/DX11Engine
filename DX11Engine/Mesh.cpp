#include "Mesh.h"

Mesh::Mesh(int vCount, SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs)
{
	vBuffer = Buffer::CreateBuffer<SimpleVertex>(verts, vCount, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<int>(triangles, tCount, BufferBindFlag::Index);
}

Mesh::Mesh(Buffer* vBuffer, Buffer* iBuffer)
{
	this->vBuffer = vBuffer;
	this->iBuffer = iBuffer;
}

Mesh::~Mesh()
{
	delete vBuffer;
	delete iBuffer;
}
