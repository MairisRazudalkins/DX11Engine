#pragma once

class Rotator
{
public:
	float x, y, z;

	Rotator() : x(0.f), y(0.f), z(0.f) {}
	Rotator(float x, float y, float z) : x(x), y(y), z(z) {}
	Rotator(const Rotator& rot) : x(rot.x), y(rot.y), z(rot.z) {}

	Rotator operator+(const Rotator& rot) { return Rotator(x + rot.x, y + rot.y, z + rot.z); }
	Rotator& operator+=(const Rotator& rot) { x += rot.x; y += rot.y; z += rot.z; return *this; }
};