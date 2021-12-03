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

	Rotator operator*(const Rotator& rot) { return Rotator(x * rot.x, y * rot.y, z * rot.z); }
	Rotator& operator*=(const Rotator& rot) { x *= rot.x; y *= rot.y; z *= rot.z; return *this; }

	Rotator operator*(const float val) { return Rotator(x * val, y * val, z * val); }
	Rotator& operator*=(const float val) { x *= val; y *= val; z *= val; return *this; }

	Rotator operator/(const Rotator& rot) { return Rotator(x / rot.x, y / rot.y, z / rot.z); }
	Rotator& operator/=(const Rotator& rot) { x /= rot.x; y /= rot.y; z /= rot.z; return *this; }

	Rotator operator/(const float val) { return Rotator(x / val, y / val, z / val); }
	Rotator& operator/=(const float val) { x /= val; y /= val; z /= val; return *this; }

	Rotator ToRadians() { return Rotator(x, y, z) * (Math::PI / 180.f); }

	std::string ToString() { char buffer[256]; sprintf_s(buffer, "X: %f Y: %f Z: %f", x, y, z); return std::string(buffer); }
	float* ToArray() { return new float[] {x, y, z}; }
};