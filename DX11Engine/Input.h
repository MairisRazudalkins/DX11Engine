#pragma once

//custom inputs
#define VK_MOUSE_UP       0xFF
#define VK_MOUSE_RIGHT     0x07 // Using dx11 reserved value.

enum KeyState : int8_t
{
	Pressed = 0,
	Released = 1,
	Repeat = 2,
	Held = 3
};

class Callback
{
public:
	Callback() = default;
	~Callback() = default;
	virtual void Call(float value = 1.f) = 0;
	virtual bool ShouldCall(unsigned char key, KeyState state = KeyState::Pressed) = 0;
};

template<typename T>
class AxisCallback : public Callback
{
	KeyState state;
	float scale = 1.f;
	bool bIsHeld = false;
	unsigned char key;

	T* object;
	void(T::* callback)(float);

public:
	AxisCallback(float scale, unsigned char key, KeyState state, T* object, void(T::* callback)(float))
	{
		this->scale = scale;
		this->key = key;
		this->state = state;
		this->object = object;
		this->callback = callback;
	}

	virtual ~AxisCallback() { delete object; }

	void Call(float value = 1.f) override { if (object) (object->*callback)(value * scale); }
	bool ShouldCall(unsigned char key, KeyState state) override 
	{ 
		if (this->key == key && this->state == KeyState::Held)
		{
			if (state == KeyState::Pressed) // TODO: FIX the engine loop callback so it looks up inputs -> or update from engine loop and peek all key states 
				bIsHeld = true;
			else if (state == KeyState::Released)
				bIsHeld = false;

			return bIsHeld;
		}

		return this->key == key;
	}
};

template <typename T>
class ActionCallback : public Callback
{
	//InputDevice inputDevice;
	KeyState state;
	unsigned char key;

	T* object;
	void(T::* callback)();

public:
	ActionCallback(unsigned char key, KeyState state, T* object, void(T::* callback)())
	{
		//this->inputDevice = inputDevice;
		this->key = key;
		this->state = state;
		this->object = object;
		this->callback = callback;
	}

	virtual ~ActionCallback() { delete object; }

	void Call(float value = 1.f) override { if (object) (object->*callback)(); }
	bool ShouldCall(unsigned char key, KeyState state) override { return (this->key == key && this->state == state); }
};

class Input
{
	Input() { RegisterRawInput(); }
	~Input();

	void RegisterRawInput(); // Register mouse raw input so WM_INPUT event is dispatched by windows.
	void TrapCursor(); // Trap cursor within the window. Useful for first person camera movement since using raw input to rotate camera TODO: Fix issue when tabbed out!

	static Input* inst;

	bool bIsCursorFocused = false;

	int x = 0, y = 0;
	std::vector<Callback*> actionBinds;
	std::vector<Callback*> axisBinds;

public:
	template<class T>
	void BindAction(unsigned char key, KeyState state, T* object, void(T::* callback)()) { actionBinds.push_back(new ActionCallback<T>(key, state, object, callback)); }

	template<class T>
	void BindAxis(unsigned char key, float scale, T* object, void(T::* callback)(float), KeyState state = KeyState::Held) { axisBinds.push_back(new AxisCallback<T>(scale, key, state, object, callback)); }

	void RecieveInput(unsigned char key, KeyState state, float value = 1.f);
	void RecieveRawMouseInput(int x, int y);

	void FocusCursor(bool bShouldTrap);

	Vector2 GetMousePos() { return Vector2(x, y); }

	static Input* GetInst() { return inst == nullptr ? inst = new Input() : inst; }
};