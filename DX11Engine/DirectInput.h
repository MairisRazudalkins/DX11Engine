#pragma once

#ifndef INPUT
#define INPUT

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

enum CursorFocus
{
	Game,
	Engine
};

enum DirectKeyState
{
	DirectReleased,
	DirectPressed,
	DirectHeld,
	NONE
};

enum DirectMouseButton
{
	LeftButton,
	RightButton,
	MiddleButton,
	Button4,
	Y,
	X
};

class DirectCallback
{

public:
	DirectCallback() = default;
	DirectCallback(unsigned char key, DirectKeyState state, bool bIsKeyboard) : key(key), state(state), bIsKeyboard(bIsKeyboard) {};

	virtual void Call(float value = 1.f) = 0;

	unsigned char key;
	bool bIsKeyboard;

	DirectKeyState state;
};

template<typename T>
class DirectActionCallback : public DirectCallback
{
	T* object;
	void(T::* callback)(void);

public:
	DirectActionCallback(unsigned char key, DirectKeyState state, bool bIsKeyboard, T* object, void(T::* callback)()) : DirectCallback(key, state, bIsKeyboard)
	{
		this->object = object;
		this->callback = callback;
	}

	virtual ~DirectActionCallback()
	{
		object = nullptr;
	}

	void Call(float value) override { if (object) (object->*callback)(); }
};

template<typename T>
class DirectAxisCallback : public DirectCallback
{
	T* object;
	void(T::* callback)(float);

	float scale;

public:
	DirectAxisCallback(float scale, unsigned char key, DirectKeyState state, bool bIsKeyboard, T* object, void(T::* callback)(float)) : DirectCallback(key, state, bIsKeyboard)
	{
		this->scale = scale;
		this->object = object;
		this->callback = callback;
	}

	virtual ~DirectAxisCallback()
	{
		object = nullptr;
	}

	void Call(float value) override { if (object) (object->*callback)(value * scale); }
};

class DirectInput
{
	static DirectInput* inst;

	bool keyboardState[256];
	bool prevKeyboardState[256];

	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevMouseState;

	LPDIRECTINPUT8 dxInput;

	CursorFocus cursorFocus = CursorFocus::Game;

	RECT defaultCursorTrap;
	Vector2 mousePos;

	std::vector<DirectCallback*> actionBinds;
	std::vector<DirectCallback*> axisBinds;

	std::vector<DirectCallback*> engineActionBinds;

	DirectInput();
	~DirectInput();

	void DispatchActionBinds();
	void DispatchAxisBinds();

	DirectKeyState GetKeyState(unsigned char key, bool bIsKeyboard);

public:
	static DirectInput* GetInst() { return inst == nullptr ? inst = new DirectInput() : inst; }
	static Vector2 GetMousePos() { return GetInst()->mousePos; }

	// ENGINE

	template<class T>
	static void BindEngineKeyboardAction(unsigned char key, DirectKeyState state, T* object, void(T::* callback)()) { GetInst()->engineActionBinds.push_back(new DirectActionCallback<T>(key, state, true, object, callback)); }

	template<class T>
	static void BindEngineMouseAction(DirectMouseButton key, DirectKeyState state, T* object, void(T::* callback)()) { GetInst()->engineActionBinds.push_back(new DirectActionCallback<T>(key, state, false, object, callback)); }

	// ENGINE

	template<class T>
	static void BindKeyboardAction(unsigned char key, DirectKeyState state, T* object, void(T::* callback)()) { GetInst()->actionBinds.push_back(new DirectActionCallback<T>(key, state, true, object, callback)); }

	template<class T>
	static void BindKeyboardAxis(unsigned char key, float scale, T* object, void(T::* callback)(float), DirectKeyState state = DirectKeyState::DirectHeld) { GetInst()->axisBinds.push_back(new DirectAxisCallback<T>(scale, key, state, true, object, callback)); }

	template<class T>
	static void BindMouseAction(DirectMouseButton key, DirectKeyState state, T* object, void(T::* callback)()) { GetInst()->actionBinds.push_back(new DirectActionCallback<T>(key, state, false, object, callback)); }

	template<class T>
	static void BindMouseAxis(DirectMouseButton key, float scale, T* object, void(T::* callback)(float), DirectKeyState state = DirectKeyState::DirectHeld) { GetInst()->axisBinds.push_back(new DirectAxisCallback<T>(scale, key, state, false, object, callback)); }

	static bool IsKeyDown(unsigned char key) { return GetInst()->keyboardState[key]; }
	static bool IsKeyUp(unsigned char key) { return !IsKeyDown(key); }

	static bool IsMouseDown(DirectMouseButton key) { return key < 4 ? GetInst()->mouseState.rgbButtons[key] : false; }
	static bool IsMouseUp(DirectMouseButton key) { return !IsMouseDown(key); }

	static void SetCursorFocus(CursorFocus focus);
	static CursorFocus GetCursorFocus() { return GetInst()->cursorFocus; }

	void Update();

	Vector2 GetRawInput() { return Vector2(mouseState.lX, mouseState.lY); }

	void TrapCursor();
	void ReleaseCursor();

	void SetCursorVisibility(bool bIsVisible);
};
#endif
