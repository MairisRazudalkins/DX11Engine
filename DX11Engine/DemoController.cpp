#include "CoreMinimal.h"
#include "DemoController.h"
#include "Camera.h"
#include "DirectInput.h"
#include "Time.h"

DemoController::DemoController() : Controller()
{
	mainCam = new Camera(Vector3(), Rotator());
	topDownCam = new Camera(Vector3(0.f, 10.f, 10.f), Rotator(90.f, 180.f, 0.f));

	SetupInputBinds();
}

DemoController::~DemoController()
{
	delete mainCam;
	delete topDownCam;
}

void DemoController::SwitchCamera()
{
	float curCamPosX = Camera::GetActiveCamera()->GetPosition().x;
	Camera::ChangeCamera(mainCam == Camera::GetActiveCamera() ? topDownCam : mainCam);
	Vector3 newCamPos = Camera::GetActiveCamera()->GetPosition();
	Camera::GetActiveCamera()->SetPosition(Vector3(curCamPosX, newCamPos.y, newCamPos.z));
}

void DemoController::MoveRight(float value)
{
	Vector3 newPos = Camera::GetActiveCamera()->GetPosition() + Vector3(value * Time::GetDeltaTime() * camSpeed, 0.f, 0.f);
	newPos.x = Math::Clamp(newPos.x, 0.f, 15.f);

	Camera::GetActiveCamera()->SetPosition(newPos);
}

void DemoController::SetupInputBinds()
{
	DirectInput::BindKeyboardAction(DIK_Q, DirectPressed, this, &DemoController::SwitchCamera);

	DirectInput::BindKeyboardAxis(DIK_D, 1.f, this, &DemoController::MoveRight);
	DirectInput::BindKeyboardAxis(DIK_A, -1.f, this, &DemoController::MoveRight);
}