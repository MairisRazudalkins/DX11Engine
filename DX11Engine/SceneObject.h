#pragma once
#include "Object.h"

class SceneObject : public Object
{
protected:
	Transform transform;

	Vector3 forward = Vector3(0.f, 0.f, 1.f);
	Vector3 up = Vector3(0.f, 1.f, 0.f);

public:
	SceneObject(Transform transform = Transform());

	Vector3 GetPosition() { return transform.position; }
	void SetPosition(Vector3 position) { this->transform.position = position; }

	Rotator GetRotation() { return transform.rotation; }
	void SetRotation(Rotator rotation) { this->transform.rotation = rotation; }

	Vector3 GetForward() { return forward; }
	Vector3 GetRight() { return forward.Cross(up); }
	Vector3 GetUp() { return up; }

	Transform GetTransform() { return transform; }

	virtual DirectX::XMMATRIX GetMatrix();
};
