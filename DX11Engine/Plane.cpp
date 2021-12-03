#include "CoreMinimal.h"
#include "Plane.h"

#include "BaseShader.h"
#include "SimplexNoise.h"

Plane::Plane(const int numX, const int numY, Transform transform, BaseShader* shader) : Mesh(transform, shader)
{
	const int vertexSize = numX * numY, triangleSize = (numX - 1) * (numY - 1) * 6;
	int triangleIndex = 0, vertexIndex = 0;

	SimplexNoise* simplexNoise = new SimplexNoise();

	GraphicsCore::SimpleVertex* vertices = new GraphicsCore::SimpleVertex[vertexSize];
	WORD* indices = new WORD[triangleSize];

	for (int y = 0; y < numY; y++)
	{
		for (int x = 0; x < numX; x++)
		{
			float noiseVal = simplexNoise->GetNoise3D(x, y, 1.f, 6, 1.8f, 0.4f, 1.f);
			vertices[vertexIndex] = { DirectX::XMFLOAT3(x - (float)(numX / 2.f), noiseVal, y - (float)(numY / 2.f)), DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(x / numX - 1, y / numY - 1) };

			if (x < numX - 1 && y < numY - 1)
			{
				AddTriangles(indices, triangleIndex, vertexIndex + numX + 1, vertexIndex, vertexIndex + numX);
				AddTriangles(indices, triangleIndex, vertexIndex, vertexIndex + numX + 1, vertexIndex + 1);
			}

			vertexIndex++;
		}
	}

	delete simplexNoise;

	vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(vertices, vertexSize, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<WORD>(indices, triangleSize, BufferBindFlag::Index);

	shader->Initialize(this);
}

void Plane::AddTriangles(WORD* indices, int& triangleIndex, int a, int b, int c)
{
	indices[triangleIndex] = a;
	indices[triangleIndex + 1] = b;
	indices[triangleIndex + 2] = c;
	triangleIndex += 3;
}