#pragma once
#include "SceneObject.h"

class Light : public SceneObject
{

protected:
	Color diffuseColor = Color(1.f, 1.f, 1.f, 1.f);
	Color ambientColor = Color(1.f, 1.f, 1.f, 1.f);

	float brightness = 1.f;

public:
	Light();
	Light(Transform transform, float brightness, Color color); // TODO: Replace Vector3 with color class once implemented.
	~Light() = default;
};