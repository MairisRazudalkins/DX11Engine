#pragma once

namespace Math
{
	const float PI = 3.14159265359f;

	inline float Clamp(float val, float min, float max) { return  val < min ? min : val < max ? val : max; }

	inline float Fade(float val) { return val * val * val * (val * (val * 6 - 15) + 10); }
	inline float Lerp(float a, float b, float t) { return a + t * (b - a); }

	inline float RandRange(float min, float max) { srand(time(NULL)); return min + rand() % (int)((max + 1) - min);
	}
}