#pragma once

class Vector2
{

public:
	float x, y;

	Vector2() : x(0.f), y(0.f) {}
	Vector2(float x, float y) : x(x), y(y) {}

	std::string ToString() { char buffer[256]; sprintf_s(buffer, "X: %f Y: %f", x, y); return std::string(buffer); }
};