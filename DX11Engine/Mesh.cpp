#include "CoreMinimal.h"
#include "GraphicsCore.h"
#include "Mesh.h"

Mesh::Mesh(int vCount, GraphicsCore::SimpleVertex* verts, int tCount, int* triangles, Vector2* uvs) : Object()
{
	vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(verts, vCount, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<int>(triangles, tCount, BufferBindFlag::Index);
}

Mesh::Mesh(Buffer* vBuffer, Buffer* iBuffer) : Object()
{
	this->vBuffer = vBuffer;
	this->iBuffer = iBuffer;
}

Mesh::Mesh(GraphicsCore::MeshData data)
{
	this->vBuffer = data.vBuffer;
	this->iBuffer = data.iBuffer;
}

Mesh::~Mesh()
{
	delete vBuffer;
	delete iBuffer;
}
