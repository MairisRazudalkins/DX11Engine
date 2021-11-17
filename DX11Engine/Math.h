#pragma once

namespace Math
{
	inline float Clamp(float val, float min, float max) { return  val < min ? min : val > max ? max : val; }
}