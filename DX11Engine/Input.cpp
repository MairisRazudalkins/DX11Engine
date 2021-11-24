#include "CoreMinimal.h"
#include "Input.h"

#include "Application.h"
#include "Graphics.h"

Input* Input::inst = nullptr;

Input::~Input()
{
	for (int i = 0; i < actionBinds.size(); i++) 
		delete actionBinds.at(i);

	for (int i = 0; i < axisBinds.size(); i++)
		delete axisBinds.at(i);
}

void Input::RegisterRawInput()
{
	#ifndef HID_USAGE_PAGE_GENERIC
		#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
	#endif
	
	#ifndef HID_USAGE_GENERIC_MOUSE
		#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
	#endif

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = Application::GetHWND();
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void Input::RecieveInput(unsigned char key, KeyState state, float value)
{
	//std::string output = "Input: "; // TODO: Move this to Logger class once created
	//output += state == KeyState::Released ? "Released: " : state == KeyState::Pressed ? "Pressed: " : "Repeat: ";
	//output += key;
	//output += "\n";
	//OutputDebugStringA(output.c_str());

	for (int i = 0; i < actionBinds.size(); i++) // loop through action bindings
	{
		if (actionBinds.at(i)->ShouldCall(key, state))
			actionBinds.at(i)->Call();
	}

	for (int i = 0; i < axisBinds.size(); i++) // loop through action bindings
	{
		if (axisBinds.at(i)->ShouldCall(key))
			axisBinds.at(i)->Call(value);
	}

}

void Input::RecieveRawMouseInput(int x, int y) 
{
	//std::string output = "Input: MouseMove: ";
	//output += std::to_string(x);
	//output += " " + std::to_string(y);
	//output += "\n";
	//OutputDebugStringA(output.c_str());

	if (bIsCursorFocused) 
		TrapCursor();

	for (int i = 0; i < axisBinds.size(); i++) // loop through axis bindings
	{
		if (axisBinds.at(i)->ShouldCall(VK_MOUSE_UP))
			axisBinds.at(i)->Call(y);
		else if (axisBinds.at(i)->ShouldCall(VK_MOUSE_RIGHT))
			axisBinds.at(i)->Call(x);
	}

	//Logger::ENGINE_LOG(Logger::Info, "X:", x, " Y:", y);
}

void Input::TrapCursor()
{
	RECT rc;
	GetClientRect(Application::GetHWND(), &rc);
	MapWindowPoints(Application::GetHWND(), nullptr, (POINT*)(&rc), 2);
	ClipCursor(&rc);
}

void Input::FocusCursor(bool bShouldTrap)
{
	this->bIsCursorFocused = bShouldTrap;
	ShowCursor(!bShouldTrap);
}