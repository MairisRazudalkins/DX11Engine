#include "Input.h"

Input* Input::inst = nullptr;

void Input::RecieveInput(unsigned char key, KeyState state)
{
	std::string output = "Input: "; // TODO: Move this to Logger class once created
	output += state == KeyState::Released ? "Released: " : state == KeyState::Pressed ? "Pressed: " : "Repeat: ";
	output += key;
	output += "\n";
	OutputDebugStringA(output.c_str());

	for (int i = 0; i < inputBinds.size(); i++)
	{
		if (inputBinds.at(i)->ShouldCall(key, state))
			inputBinds.at(i)->Call();
	}
}

void Input::RecieveMouseMove(int x, int y)
{
	std::string output = "Input: MouseMove: ";
	output += std::to_string(x);
	output += " " + std::to_string(y);
	output += "\n";
	OutputDebugStringA(output.c_str());

	this->x = x;
	this->y = y;
}