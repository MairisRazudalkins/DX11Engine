#pragma once
#include "Object.h"

class SceneObject : public Object
{
	Rotator rotation;
	Vector3 position;
	Vector3 scale;

	Vector3 forward = Vector3(0.f, 0.f, 1.f);
	Vector3 up = Vector3(0.f, 1.f, 0.f);

public:
	SceneObject(Vector3 position = Vector3(), Rotator rotation = Rotator(), Vector3 scale = Vector3());

	Vector3 GetPosition() { return position; }
	void SetPosition(Vector3 position)
	{
		this->position = position;
		Logger::ENGINE_LOG(Logger::Info, position.ToString());
	}

	Rotator GetRotation() { return rotation; }
	void SetRotation(Rotator rotation) { this->rotation = rotation; }

	Vector3 GetForward() { return forward; }
	Vector3 GetRight() { return forward.Cross(up); }
	Vector3 GetUp() { return up; }

	DirectX::XMMATRIX GetMatrix();
};
