#include "CoreMinimal.h"
#include "SkyDome.h"
#include "SkyDomeShader.h"
#include "OBJLoader.h"

SkyDome::SkyDome() : Mesh(Transform(Vector3(), Rotator(), Vector3(200.f, 200.f, 200.f)), OBJLoader::Load("Assets/Models/SkyDome.obj", false), new SkyDomeShader(this))
{
	
}

SkyDome::~SkyDome()
{
	
}