#pragma once

class Vector3
{

public:
	float x, y, z;

	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(float xyz) : x(xyz), y(xyz), z(xyz) {}
	Vector3(DirectX::XMFLOAT3 pos) : x(pos.x), y(pos.y), z(pos.z) {}
	Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z) {}

	Vector3 operator+(const Vector3& vec) { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	Vector3& operator+=(const Vector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }

	Vector3 operator-(const Vector3& vec) { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	Vector3& operator-=(const Vector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }

	Vector3 operator*(const Vector3& vec) { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	Vector3& operator*=(const Vector3& vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; }

	Vector3 operator*(const float val) { return Vector3(x * val, y * val, z * val); }
	Vector3& operator*=(const float val) { x *= val; y *= val; z *= val; return *this; }

	Vector3 operator/(const Vector3& vec) { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
	Vector3& operator/=(const Vector3& vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this;}

	Vector3 operator/(const float val) { return Vector3(x / val, y / val, z / val); }
	Vector3& operator/=(const float val) { x /= val; y /= val; z /= val; return *this; }


	Vector3 Cross(const Vector3& vec) { return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
	Vector3 Normalize() { float magnitude = Magnitude(); return Vector3(x, y, z) / magnitude; }

	float Square() { return x * x + y * y + z * z; }
	float Distance(const Vector3& vec) {}
	float Dot(const Vector3& vec) { return x * vec.x + y * vec.y + z * vec.z; }
	float Magnitude() { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

	std::string ToString() { char buffer[256]; sprintf_s(buffer, "X: %f Y: %f Z: %f", x, y, z); return std::string(buffer); } // weird way of formating a string. Couldnt use <format>
	float* ToArray() { return new float[] { x,y,z }; }
	DirectX::XMFLOAT3 ToFloat3() { return DirectX::XMFLOAT3(x, y, z); }
};