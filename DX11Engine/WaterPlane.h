#pragma once
#include "Plane.h"
class WaterPlane : public Plane
{

public:
	WaterPlane(const int numX, const int numY, Transform transfrom = Transform());
	~WaterPlane();
};

