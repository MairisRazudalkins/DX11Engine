#include "CoreMinimal.h"
#include "Orbit.h"

#include "LitShader.h"
#include "OBJLoader.h"
#include "TexturedShader.h"

#include "Time.h"

Orbit::Orbit(Transform transform) : Mesh(transform, OBJLoader::Load("Assets/Models/Sphere.obj"), new TexturedShader(L"Assets/Textures/SunTexture.dds"))
{
	meshes[0] = new Mesh(Transform(Vector3(0.f, 0.f, 3.f), Rotator(), Vector3(0.3f, 0.3f, 0.3f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SkySphere.dds"));
	meshes[0]->SetParent(this);

	meshes[1] = new Mesh(Transform(Vector3(3.f, 0.f, -5.f), Rotator(), Vector3(0.5f, 0.5f, 0.5f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SkySphere.dds"));
	meshes[1]->SetParent(this);

	meshes[2] = new Mesh(Transform(Vector3(0.f, 0.f, 3.f), Rotator(), Vector3(0.6f, 0.6f, 0.6f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/MoonTexture.dds"));
	meshes[2]->SetParent(meshes[0]);
		   
	meshes[3] = new Mesh(Transform(Vector3(2.f, 0.f, 5.f), Rotator(), Vector3(0.6f, 0.6f, 0.6f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SkySphere.dds"));
	meshes[3]->SetParent(meshes[1]);

	meshes[4] = new Mesh(Transform(Vector3(3.f, 0.f, 2.f), Rotator(), Vector3(0.6f, 0.6f, 0.6f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/MoonTexture.dds"));
	meshes[4]->SetParent(meshes[3]);

	meshes[5] = new Mesh(Transform(Vector3(2.f, 0.f, 5.f), Rotator(), Vector3(0.6f, 0.6f, 0.6f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/MoonTexture.dds"));
	meshes[5]->SetParent(meshes[1]);

	meshes[6] = new Mesh(Transform(Vector3(3.f, 0.f, 2.f), Rotator(), Vector3(0.6f, 0.6f, 0.6f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SunTexture.dds"));
	meshes[6]->SetParent(meshes[5]);

	meshes[7] = new Mesh(Transform(Vector3(5.f, 0.f, 8.f), Rotator(), Vector3(0.4f, 0.4f, 0.4f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SunTexture.dds"));
	meshes[7]->SetParent(meshes[1]);

	meshes[8] = new Mesh(Transform(Vector3(5.f, 0.f, 2.f), Rotator(), Vector3(0.5f, 0.5f, 0.5f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/SkySphere.dds"));
	meshes[8]->SetParent(meshes[0]);

	meshes[9] = new Mesh(Transform(Vector3(4.f, 0.f, 2.f), Rotator(), Vector3(0.7f, 0.7f, 0.7f)), vBuffer, iBuffer, new TexturedShader(L"Assets/Textures/MoonTexture.dds"));
	meshes[9]->SetParent(meshes[8]);

	for (int i = 0; i < PLANET_COUNT; i++)
	{
		rotations[i] = Rotator(Math::RandRange(7.f, 50.f), Math::RandRange(7.f, 50.f), Math::RandRange(7.f, 50.f));
	}
}

Orbit::~Orbit()
{
	for (int i = 0; i < PLANET_COUNT; i++)
		delete meshes[i];
}

void Orbit::Render(ID3D11DeviceContext* deviceContext, UINT& offset)
{
	Mesh::Render(deviceContext, offset);

	for (int i = 0; i < PLANET_COUNT; i++)
		if (meshes[i])
			meshes[i]->Render(deviceContext, offset);
}

void Orbit::Update()
{
	Mesh::Update();

	SetRotation(GetRotation() + Rotator(Time::GetDeltaTime() * 20.f, Time::GetDeltaTime() * 30.f, 0.f));

	for (int i = 0; i < PLANET_COUNT; i++)
		if (meshes[i])
			meshes[i]->SetRotation(meshes[i]->GetRotation() + rotations[i] * Time::GetDeltaTime());
}
