#pragma once
#include "CoreMinimal.h"

enum KeyState : int8_t
{
	Pressed = 0,
	Released = 1,
	Repeat = 2
};

class Callback
{
public:
	Callback() = default;
	virtual void Call() = 0;
	virtual bool ShouldCall(unsigned char key, KeyState state) = 0;
};

template <typename T>
class InputCallback : public Callback
{
	//InputDevice inputDevice;
	KeyState state;
	unsigned char key;

	T* object;
	void(T::* callback)();

public:
	InputCallback(unsigned char key, KeyState state, T* object, void(T::* callback)())
	{
		//this->inputDevice = inputDevice;
		this->key = key;
		this->state = state;
		this->object = object;
		this->callback = callback;
	}

	void Call() override
	{
		if (object)
			(object->*callback)();
	}

	bool ShouldCall(unsigned char key, KeyState state) override
	{
		return (this->key == key && this->state == state);
	}
};

class Input
{
	Input() = default;
	~Input() { for (int i = 0; i < inputBinds.size(); i++) delete inputBinds.at(i); }

	static Input* inst;

	int x = 0, y = 0;
	std::vector<Callback*> inputBinds;

public:
	template<class T>
	void BindAction(unsigned char key, KeyState state, T* object, void(T::* callback)()) { inputBinds.push_back(new InputCallback<T>(key, state, object, callback)); }

	void RecieveInput(unsigned char key, KeyState state);
	void RecieveMouseMove(int x, int y);

	Vector2 GetMousePos() { return Vector2(x, y); }

	static Input* GetInst() { return inst == nullptr ? inst = new Input() : inst; }
};
