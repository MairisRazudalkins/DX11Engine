#pragma once
#include "Vector3.h"
#include "Rotator.h"

class Transform 
{
public:
	Vector3 position;
	Vector3 scale;
	Rotator rotation;

	Transform(Vector3 position = Vector3(), Rotator rotation = Rotator(), Vector3 scale = Vector3()) 
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
};