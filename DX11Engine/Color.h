#pragma once
#include <string>

class Color
{
public:
	float r, g, b, a;

	Color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f) : r(r), g(g), b(b), a(a) {};

	Color RGBHex(int hexValue) { return Color(((hexValue >> 16) & 0xFF) / 255.f, ((hexValue >> 8) & 0xFF) / 255.f, ((hexValue) & 0xFF) / 255.f); }
	Color RGBAHex(int hexValue) { return Color(((hexValue >> 24) & 0xFF) / 255.f, ((hexValue >> 16) & 0xFF) / 255.f, ((hexValue >> 8) & 0xFF) / 255.f, ((hexValue) & 0xFF) / 255.f); }
};