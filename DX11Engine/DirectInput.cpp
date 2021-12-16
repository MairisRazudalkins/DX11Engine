#include "CoreMinimal.h"
#include "DirectInput.h"

#include "Application.h"

DirectInput* DirectInput::inst = nullptr;

DirectInput::DirectInput()
{
	HINSTANCE hInstance = Application::GetHInstance();

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dxInput, NULL))) //Create Input
		Quit("Failed To create direct input");

	if (FAILED(dxInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL))) // Create Keyboard
		Quit("Failed To create keyboard input");

	if (FAILED(dxInput->CreateDevice(GUID_SysMouse, &mouse, NULL))) // Create Mouse
		Quit("Failed To create mouse input");

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(Application::GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(Application::GetHWND(), DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	GetClipCursor(&defaultCursorTrap);
}

DirectInput::~DirectInput()
{
	for (int i = 0; i < actionBinds.size(); i++)
		delete actionBinds.at(i);

	for (int i = 0; i < axisBinds.size(); i++)
		delete axisBinds.at(i);

	for (int i = 0; i < engineActionBinds.size(); i++)
		delete engineActionBinds.at(i);

	keyboard->Unacquire();
	keyboard->Release();
	mouse->Unacquire();
	mouse->Release();

	dxInput->Release();

	delete inst;
	inst = nullptr;
}

void DirectInput::Update()
{
	keyboard->Acquire();
	mouse->Acquire();

	std::memcpy(prevKeyboardState, keyboardState, sizeof(keyboardState)); // get around an issue with assigning error
	std::memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
	
	keyboard->GetDeviceState(sizeof(keyboardState), &keyboardState);
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

	DispatchActionBinds();
	DispatchAxisBinds();
}

void DirectInput::DispatchActionBinds()
{
	if (cursorFocus == CursorFocus::Game)
	{
		for (int i = 0; i < actionBinds.size(); i++)
		{
			DirectCallback* callback = actionBinds[i];

			if (callback->bIsKeyboard)
			{
				if (actionBinds[i]->state == GetKeyState(actionBinds[i]->key, true))
					callback->Call();
			}
			else if (actionBinds[i]->state == GetKeyState(actionBinds[i]->key, false))
				callback->Call();
		}
	}

	for (int i = 0; i < engineActionBinds.size(); i++)
	{
		DirectCallback* callback = engineActionBinds[i];

		if (callback->bIsKeyboard)
		{
			if (engineActionBinds[i]->state == GetKeyState(engineActionBinds[i]->key, true))
				callback->Call();
		}
		else if (engineActionBinds[i]->state == GetKeyState(engineActionBinds[i]->key, false))
			callback->Call();
	}
}

void DirectInput::DispatchAxisBinds()
{
	if (cursorFocus == CursorFocus::Game)
	{
		for (size_t i = 0; i < axisBinds.size(); i++)
		{
			DirectCallback* callback = axisBinds[i];

			if (callback->bIsKeyboard)
			{
				if (keyboardState[axisBinds[i]->key] && axisBinds[i]->state == GetKeyState(axisBinds[i]->key, true))
					callback->Call(1.f);
			}
			else if (mouseState.rgbButtons[axisBinds[i]->key] && axisBinds[i]->state == GetKeyState(axisBinds[i]->key, false))
				callback->Call(1.f);
			else if (callback->key == DirectMouseButton::X)
				callback->Call(GetRawInput().x);
			else if (callback->key == DirectMouseButton::Y)
				callback->Call(GetRawInput().y);
		}
	}

	//Logger::ENGINE_LOG(Logger::Info, "PREV:", prevKeyboardState[DIK_W], "CUR:", keyboardState[DIK_W]);
	//
	
	//Logger::ENGINE_LOG(Logger::Info, GetRawInput().ToString());
}

void DirectInput::TrapCursor()
{
	mouse->Unacquire();
	//int i = ShowCursor(FALSE);

	SetCursorVisibility(false);

	RECT rc;
	GetClientRect(Application::GetHWND(), &rc);
	MapWindowPoints(Application::GetHWND(), nullptr, (POINT*)(&rc), 2);
	ClipCursor(&rc);

	//Logger::ENGINE_LOG(Logger::Info, "TRAP VAL",i);

	//SetCursor(NULL);
}

void DirectInput::ReleaseCursor()
{
	mouse->Unacquire();
	//int i = ShowCursor(TRUE);

	SetCursorVisibility(true);

	//Logger::ENGINE_LOG(Logger::Info, "RELEASE VAL", i);

	//SetCursor(LoadCursor(Application::GetHInstance(), IDC_ARROW));
	ClipCursor(&defaultCursorTrap);
}

void DirectInput::SetCursorVisibility(bool bIsVisible) // THIS HAS TO BE THE STRANGEST WAY OF SOLVING A CURSOR VISIBILITY ISSUE.
{
	int i = ShowCursor(bIsVisible);

	if (bIsVisible)
	{
		for (int j = i; j < 0; j++)
			ShowCursor(TRUE);
	}
	else
	{
		for (int j = i; j > -1; j--)
			ShowCursor(FALSE);
	}

	Logger::ENGINE_LOG(Logger::Info, "Cursor set to : ", bIsVisible ? "Visible" : "Hidden");
}

DirectKeyState DirectInput::GetKeyState(unsigned char key, bool bIsKeyboard)
{
	if (bIsKeyboard) 
		return keyboardState[key] && prevKeyboardState[key] ? DirectKeyState::DirectHeld : keyboardState[key] && !prevKeyboardState[key] ? DirectKeyState::DirectPressed : !keyboardState[key] && prevKeyboardState[key] ? DirectKeyState::DirectReleased : DirectKeyState::NONE;

	if(key < 4) 
		return mouseState.rgbButtons[key] && prevMouseState.rgbButtons[key] ? DirectKeyState::DirectHeld : mouseState.rgbButtons[key] && !prevMouseState.rgbButtons[key] ? DirectKeyState::DirectPressed : !mouseState.rgbButtons[key] && prevMouseState.rgbButtons[key] ? DirectKeyState::DirectReleased : DirectKeyState::NONE;
}

void DirectInput::SetCursorFocus(CursorFocus focus)
{
	Logger::ENGINE_LOG(Logger::Info, "Game focus: ", focus == CursorFocus::Game ? "gained" : "lost");

	if (focus == CursorFocus::Game)
		GetInst()->TrapCursor();
	else if (focus == CursorFocus::Engine)
		GetInst()->ReleaseCursor();

	GetInst()->cursorFocus = focus;
}