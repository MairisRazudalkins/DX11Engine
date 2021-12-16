#include "CoreMinimal.h"
#include "SkyDome.h"

#include "Camera.h"
#include "SkyDomeShader.h"
#include "OBJLoader.h"

SkyDome::SkyDome() : Mesh(Transform(Vector3(), Rotator(), Vector3(200.f, 200.f, 200.f)), OBJLoader::Load("Assets/Models/SkyDome.obj", false), new SkyDomeShader(this))
{
	
}

SkyDome::~SkyDome()
{
	
}

DirectX::XMMATRIX SkyDome::GetMatrix()
{
	Vector3 camPos = Camera::GetActiveCamera()->GetPosition();
	return DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) * DirectX::XMMatrixTranslation(camPos.x, camPos.y, camPos.z); // Always stay at the position of the camera.
}
