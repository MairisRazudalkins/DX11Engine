#include "CoreMinimal.h"
#include "EngineController.h"

#include "BaseShader.h"
#include "DirectInput.h"
#include "Camera.h"
#include "Time.h"

void EngineController::SetupInputBinds()
{
	DirectInput::BindEngineKeyboardAction(DIK_F8, DirectKeyState::DirectPressed, this, &EngineController::ToggleFocus);

	DirectInput::BindEngineKeyboardAction(DIK_1, DirectKeyState::DirectPressed, this, &EngineController::SetResetGlobalRasterizer);
	DirectInput::BindEngineKeyboardAction(DIK_2, DirectKeyState::DirectPressed, this, &EngineController::SetGlobalRasterToWireframe);
	
	DirectInput::BindEngineMouseAction(DirectMouseButton::RightButton, DirectKeyState::DirectPressed, this, &EngineController::Trap);
	DirectInput::BindEngineMouseAction(DirectMouseButton::RightButton, DirectKeyState::DirectReleased, this, &EngineController::Release);
	
	DirectInput::BindEngineMouseAction(DirectMouseButton::RightButton, DirectKeyState::DirectHeld, this, &EngineController::EngineFocus);

	engineCamera = new Camera();
}

EngineController::EngineController()
{
	SetupInputBinds();
}

EngineController::~EngineController()
{
	delete engineCamera;
	gameCamera = nullptr;
}

void EngineController::Trap()
{
	if (DirectInput::GetCursorFocus() == CursorFocus::Engine)
		DirectInput::GetInst()->TrapCursor();
}

void EngineController::Release()
{
	if (DirectInput::GetCursorFocus() == CursorFocus::Engine)
		DirectInput::GetInst()->ReleaseCursor();
}

void EngineController::ToggleFocus()
{
	CursorFocus newFocus = DirectInput::GetCursorFocus() == CursorFocus::Game ? CursorFocus::Engine : CursorFocus::Game;

	DirectInput::SetCursorFocus(newFocus);

	if (newFocus == CursorFocus::Game)
	{
		Camera::ChangeCamera(gameCamera == engineCamera ? nullptr : gameCamera);
		DirectInput::GetInst()->TrapCursor();
	}
	else if (newFocus == CursorFocus::Engine)
	{
		gameCamera = Camera::GetActiveCamera();

		engineCamera->SetPosition(gameCamera->GetPosition());
		engineCamera->SetRotation(gameCamera->GetRotation());

		Camera::ChangeCamera(engineCamera);
	}
}

void EngineController::EngineFocus()
{
	if (DirectInput::GetCursorFocus() == CursorFocus::Engine)
	{
		Vector2 rawInput = DirectInput::GetInst()->GetRawInput();
		float scrollerInput = DirectInput::GetInst()->GetRawScollerInput();

		if (scrollerInput != 0.f)
			camSpeed = Math::Clamp((scrollerInput / 30.f) + camSpeed, 10.f, 100.f); // scrollerInput / 60.f <- scrolling up once has a value of 120.f

		if (rawInput.x != 0.f)
			engineCamera->LookRight(rawInput.x * mouseSensitivity);

		if (rawInput.y != 0.f)
			engineCamera->LookUp(rawInput.y * mouseSensitivity);

		if (DirectInput::IsKeyDown(DIK_W))
			engineCamera->MoveForward(camSpeed * Time::GetDeltaTime());
		else if (DirectInput::IsKeyDown(DIK_S))
			engineCamera->MoveForward(-camSpeed * Time::GetDeltaTime());

		if (DirectInput::IsKeyDown(DIK_A))
			engineCamera->MoveRight(camSpeed * Time::GetDeltaTime());
		else if (DirectInput::IsKeyDown(DIK_D))
			engineCamera->MoveRight(-camSpeed * Time::GetDeltaTime());
	}
}

void EngineController::SetGlobalRasterToWireframe()
{
	BaseShader::SetGlobalRasterizer(D3D11_FILL_WIREFRAME);
}

void EngineController::SetResetGlobalRasterizer()
{
	BaseShader::SetGlobalRasterizer(D3D11_FILL_SOLID);
}