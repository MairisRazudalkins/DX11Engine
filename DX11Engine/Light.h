#pragma once
#include "SceneObject.h"

class Light : public SceneObject
{
protected:
	Vector3 color;

	float brightness = 1.f;

public:
	Light(Transform transform, float brightness, Vector3 color); // TODO: Replace Vector3 with color class once implemented.
	~Light();
};