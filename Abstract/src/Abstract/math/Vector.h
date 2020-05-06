#pragma once
#include "../core/Platform.h"
#include <cmath>

namespace Abstract {

	class ABSTRACT_API Vector3
	{
	public:
		Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) { }

		constexpr float getX() const { return x; }
		constexpr float getY() const { return y; }
		constexpr float getZ() const { return z; }

		constexpr void setX(float x) { this->x = x; }
		constexpr void setY(float y) { this->y = y; }
		constexpr void setZ(float z) { this->z = z; }

		inline float length() const { return sqrtf(x * x + y * y + z * z); }
		inline Vector3 normalized() const { return this->operator/(length()); }

		constexpr float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
		inline Vector3 cross(const Vector3& other) const
		{
			float _x = y * other.z - z * other.y;
			float _y = z * other.x - x * other.z;
			float _z = x * other.y - y * other.x;

			return Vector3(_x, _y, _z);
		}

		inline float min() const { return fminf(x, fminf(y, z)); }
		inline float max() const { return fmaxf(x, fmaxf(y, z)); }

#pragma region Operator Overloads

		inline Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		inline Vector3 operator+(const float& other) const
		{
			return Vector3(x + other, y + other, z + other);
		}

		inline Vector3 operator+(const Vector3& other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		inline Vector3 operator-(const float& other) const
		{
			return Vector3(x - other, y - other, z - other);
		}

		inline Vector3 operator-(const Vector3& other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		inline Vector3 operator*(const float& other) const
		{
			return Vector3(x * other, y * other, z * other);
		}

		inline Vector3 operator*(const Vector3& other) const
		{
			return Vector3(x * other.x, y * other.y, z * other.z);
		}

		inline Vector3 operator/(const float& other) const
		{
			return Vector3(x / other, y / other, z / other);
		}

		inline Vector3 operator/(const Vector3& other) const
		{
			return Vector3(x / other.x, y / other.y, z / other.z);
		}

		constexpr void operator+=(const float& other)
		{
			this->x += other;
			this->y += other;
			this->z += other;
		}

		constexpr void operator+=(const Vector3& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
		}

		constexpr void operator-=(const float& other)
		{
			this->x -= other;
			this->y -= other;
			this->z -= other;
		}

		constexpr void operator-=(const Vector3& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
		}

		constexpr void operator*=(const float& other)
		{
			this->x *= other;
			this->y *= other;
			this->z *= other;
		}

		constexpr void operator*=(const Vector3& other)
		{
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
		}

		constexpr void operator/=(const float& other)
		{
			this->x /= other;
			this->y /= other;
			this->z /= other;
		}

		constexpr void operator/=(const Vector3& other)
		{
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
		}

#pragma endregion
		~Vector3() { }
	private:
		float x;
		float y;
		float z;
	};

	class ABSTRACT_API Vector2
	{
	public:
		Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) { }

		constexpr float getX() const { return x; }
		constexpr float getY() const { return y; }

		constexpr void setX(float x) { this->x = x; }
		constexpr void setY(float y) { this->y = y; }

		inline float length() const { return sqrtf(x * x + y * y); }
		inline Vector2 normalized() const { return this->operator/(length()); }

		constexpr float dot(const Vector2& other) const { return x * other.x + y * other.y; }
		constexpr float cross(const Vector2& other) const { return x * other.y - y * other.x; }

		inline float min() { return fminf(x, y); }
		inline float max() { return fmaxf(x, y); }

#pragma region Operator Overloads

		inline Vector2 operator+(const float& other) const
		{
			return Vector2(x + other, y + other);
		}

		inline Vector2 operator+(const Vector2& other) const
		{
			return Vector2(x + other.x, y + other.y);
		}

		inline Vector2 operator-(const float& other) const
		{
			return Vector2(x - other, y - other);
		}

		inline Vector2 operator-(const Vector2& other) const
		{
			return Vector2(x - other.x, y - other.y);
		}

		inline Vector2 operator*(const float& other) const
		{
			return Vector2(x * other, y * other);
		}

		inline Vector2 operator*(const Vector2& other) const
		{
			return Vector2(x * other.x, y * other.y);
		}

		inline Vector2 operator/(const float& other) const
		{
			return Vector2(x / other, y / other);
		}

		inline Vector2 operator/(const Vector2& other) const
		{
			return Vector2(x / other.x, y / other.y);
		}

		constexpr void operator+=(const float& other)
		{
			this->x += other;
			this->y += other;
		}

		constexpr void operator+=(const Vector2& other)
		{
			this->x += other.x;
			this->y += other.y;
		}

		constexpr void operator-=(const float& other)
		{
			this->x -= other;
			this->y -= other;
		}

		constexpr void operator-=(const Vector2& other)
		{
			this->x -= other.x;
			this->y -= other.y;
		}

		constexpr void operator*=(const float& other)
		{
			this->x *= other;
			this->y *= other;
		}

		constexpr void operator*=(const Vector2& other)
		{
			this->x *= other.x;
			this->y *= other.y;
		}

		constexpr void operator/=(const float& other)
		{
			this->x /= other;
			this->y /= other;
		}

		constexpr void operator/=(const Vector2& other)
		{
			this->x /= other.x;
			this->y /= other.y;
		}

#pragma endregion
		~Vector2() { }
	private:
		float x;
		float y;
	};

}