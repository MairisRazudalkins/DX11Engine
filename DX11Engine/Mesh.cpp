#include "CoreMinimal.h"
#include "Mesh.h"

Mesh::Mesh(int vCount, VertexData::SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs) : Object()
{
	vBuffer = Buffer::CreateBuffer<VertexData::SimpleVertex>(verts, vCount, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<int>(triangles, tCount, BufferBindFlag::Index);
}

Mesh::Mesh(Buffer* vBuffer, Buffer* iBuffer) : Object()
{
	this->vBuffer = vBuffer;
	this->iBuffer = iBuffer;
}

Mesh::~Mesh()
{
	delete vBuffer;
	delete iBuffer;
}
