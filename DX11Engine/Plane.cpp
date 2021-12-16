#include "CoreMinimal.h"
#include "Plane.h"

#include <fstream>

#include "BaseShader.h"
#include "LitShader.h"
#include "SimplexNoise.h"

Plane::Plane(const int numX, const int numY, Transform transform, BaseShader* shader) : Mesh(transform, shader)
{
	const int vertexSize = numX * numY, triangleSize = (numX - 1) * (numY - 1) * 6;
	int triangleIndex = 0, vertexIndex = 0;

	SimplexNoise* simplexNoise = new SimplexNoise();

	GraphicsCore::SimpleVertex* vertices = new GraphicsCore::SimpleVertex[vertexSize];
	WORD* indices = new WORD[triangleSize];

	float* heightMap = LoadHeightmap(numX, numY, 3.f);

	for (int y = 0; y < numY; y++)
	{
		for (int x = 0; x < numX; x++)
		{
			//float noiseVal = simplexNoise->GetNoise3D(x / numX, y / numY, 1.f);
			//float noiseVal = simplexNoise->GetNoise3D(x, y, 1.f, 6, 2.4f, 0.1f, 1.f);
			vertices[vertexIndex] = { DirectX::XMFLOAT3(x - (float)(numX / 2.f), heightMap[y * x + x] - 5.f, y - (float)(numY / 2.f)), DirectX::XMFLOAT3(0.f, 1.f, 0.f), DirectX::XMFLOAT2(x / numX - 1, y / numY - 1) };

			if (x < numX - 1 && y < numY - 1)
			{
				AddTriangles(indices, triangleIndex, vertexIndex + numX + 1, vertexIndex, vertexIndex + numX);
				AddTriangles(indices, triangleIndex, vertexIndex, vertexIndex + numX + 1, vertexIndex + 1);
			}

			vertexIndex++;
		}
	}

	CalculateNormals(vertices, (numX - 1) * (numY - 1));

	delete heightMap;
	delete simplexNoise;

	vBuffer = Buffer::CreateBuffer<GraphicsCore::SimpleVertex>(vertices, vertexSize, BufferBindFlag::Vertex);
	iBuffer = Buffer::CreateBuffer<WORD>(indices, triangleSize, BufferBindFlag::Index);

	shader->Initialize(this);

	if ((LitShader*)shader)
		((LitShader*)shader)->SetAmbientColor(DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
}

float* Plane::LoadHeightmap(int width, int height, float scale)
{
	std::vector<unsigned char> in(width * height);

	// Open the file.

	std::ifstream inFile;

	inFile.open("Assets/Textures/Heightmap513x513.raw", std::ios_base::binary);

	if (inFile)
	{
		inFile.read((char*)&in[0], (std::streamsize)in.size());
		inFile.close();
	}

	float* heightMap = new float[width * height];

	for (UINT i = 0; i < width * height; ++i)
		heightMap[i] = (in[i] / 255.0f) * scale;

	return heightMap;
}

void Plane::AddTriangles(WORD* indices, int& triangleIndex, int a, int b, int c)
{
	indices[triangleIndex] = a;
	indices[triangleIndex + 1] = b;
	indices[triangleIndex + 2] = c;
	triangleIndex += 3;
}

void Plane::CalculateNormals(GraphicsCore::SimpleVertex* verts, int count)
{
	for (int i = 0; i < count; i += 3)
	{
		Vector3 pos1 = Vector3(verts[i].pos);
		Vector3 pos2 = Vector3(verts[i + 1].pos);
		Vector3 pos3 = Vector3(verts[i + 2].pos);

		Vector3 norm = (pos2 - pos1).Cross(pos3 - pos1);
		verts[i].normal = (Vector3(verts[i].normal) + norm).ToFloat3();
		verts[i + 1].normal = (Vector3(verts[i + 1].normal) + norm).ToFloat3();
		verts[i + 2].normal = (Vector3(verts[i + 2].normal) + norm).ToFloat3();
	}

	for (int i = 0; i < count; i++)
	{
		verts[i].normal = Vector3(verts[i].normal).Normalize().ToFloat3();
	}
}