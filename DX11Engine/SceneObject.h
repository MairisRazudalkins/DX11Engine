#pragma once
#include "Object.h"

class SceneObject : public Object
{
protected:
	SceneObject* parent;

	Transform transform;

	Vector3 forward = Vector3(0.f, 0.f, 1.f);
	Vector3 up = Vector3(0.f, 1.f, 0.f);

	DirectX::XMFLOAT4X4 worldMatrix;

public:
	SceneObject(Transform transform = Transform());

	Vector3 GetPosition() { return transform.position; }
	void SetPosition(Vector3 position) { this->transform.position = position; }

	Rotator GetRotation() { return transform.rotation; }
	void SetRotation(Rotator rotation) { this->transform.rotation = rotation; }

	Vector3 GetScale() { return transform.scale; }
	void SetScale(Vector3 scale) { this->transform.scale = scale; }

	Vector3 GetForward() { return forward; }
	Vector3 GetRight() { return forward.Cross(up); }
	Vector3 GetUp() { return up; }

	Transform GetTransform() { return transform; }

	void SetParent(SceneObject* parent) { this->parent = parent; }

	virtual DirectX::XMMATRIX GetMatrix();
	virtual DirectX::XMFLOAT4X4 GetWorldMatrix();

	virtual void Update() {};
};
