#pragma once
#include "../core/Platform.h"
#include <cmath>

namespace Abstract {

	class ABSTRACT_API Vector3
	{
	public:
		Vector3(double x = 0.0f, double y = 0.0f, double z = 0.0f) : x(x), y(y), z(z) { }

		constexpr double getX() const { return x; }
		constexpr double getY() const { return y; }
		constexpr double getZ() const { return z; }

		constexpr void setX(double x) { this->x = x; }
		constexpr void setY(double y) { this->y = y; }
		constexpr void setZ(double z) { this->z = z; }

		inline double length() const { return sqrt(x * x + y * y + z * z); }
		inline Vector3 normalized() const { return this->operator/(length()); }

		constexpr double dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
		inline Vector3 cross(const Vector3& other) const
		{
			double _x = y * other.z - z * other.y;
			double _y = z * other.x - x * other.z;
			double _z = x * other.y - y * other.x;

			return Vector3(_x, _y, _z);
		}

		inline double min() const { return fmin(x, fmin(y, z)); }
		inline double max() const { return fmax(x, fmax(y, z)); }

#pragma region Operator Overloads

		inline Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		inline Vector3 operator+(const double& other) const
		{
			return Vector3(x + other, y + other, z + other);
		}

		inline Vector3 operator+(const Vector3& other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		inline Vector3 operator-(const double& other) const
		{
			return Vector3(x - other, y - other, z - other);
		}

		inline Vector3 operator-(const Vector3& other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		inline Vector3 operator*(const double& other) const
		{
			return Vector3(x * other, y * other, z * other);
		}

		inline Vector3 operator*(const Vector3& other) const
		{
			return Vector3(x * other.x, y * other.y, z * other.z);
		}

		inline Vector3 operator/(const double& other) const
		{
			return Vector3(x / other, y / other, z / other);
		}

		inline Vector3 operator/(const Vector3& other) const
		{
			return Vector3(x / other.x, y / other.y, z / other.z);
		}

		constexpr void operator+=(const double& other)
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

		constexpr void operator-=(const double& other)
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

		constexpr void operator*=(const double& other)
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

		constexpr void operator/=(const double& other)
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
		double x;
		double y;
		double z;
	};

	class ABSTRACT_API Vector2
	{
	public:
		Vector2(double x = 0.0f, double y = 0.0f) : x(x), y(y) { }

		constexpr double getX() const { return x; }
		constexpr double getY() const { return y; }

		constexpr void setX(double x) { this->x = x; }
		constexpr void setY(double y) { this->y = y; }

		inline double length() const { return sqrt(x * x + y * y); }
		inline Vector2 normalized() const { return this->operator/(length()); }

		constexpr double dot(const Vector2& other) const { return x * other.x + y * other.y; }
		constexpr double cross(const Vector2& other) const { return x * other.y - y * other.x; }

		inline double min() { return fmin(x, y); }
		inline double max() { return fmax(x, y); }

#pragma region Operator Overloads

		inline Vector2 operator+(const double& other) const
		{
			return Vector2(x + other, y + other);
		}

		inline Vector2 operator+(const Vector2& other) const
		{
			return Vector2(x + other.x, y + other.y);
		}

		inline Vector2 operator-(const double& other) const
		{
			return Vector2(x - other, y - other);
		}

		inline Vector2 operator-(const Vector2& other) const
		{
			return Vector2(x - other.x, y - other.y);
		}

		inline Vector2 operator*(const double& other) const
		{
			return Vector2(x * other, y * other);
		}

		inline Vector2 operator*(const Vector2& other) const
		{
			return Vector2(x * other.x, y * other.y);
		}

		inline Vector2 operator/(const double& other) const
		{
			return Vector2(x / other, y / other);
		}

		inline Vector2 operator/(const Vector2& other) const
		{
			return Vector2(x / other.x, y / other.y);
		}

		constexpr void operator+=(const double& other)
		{
			this->x += other;
			this->y += other;
		}

		constexpr void operator+=(const Vector2& other)
		{
			this->x += other.x;
			this->y += other.y;
		}

		constexpr void operator-=(const double& other)
		{
			this->x -= other;
			this->y -= other;
		}

		constexpr void operator-=(const Vector2& other)
		{
			this->x -= other.x;
			this->y -= other.y;
		}

		constexpr void operator*=(const double& other)
		{
			this->x *= other;
			this->y *= other;
		}

		constexpr void operator*=(const Vector2& other)
		{
			this->x *= other.x;
			this->y *= other.y;
		}

		constexpr void operator/=(const double& other)
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
		double x;
		double y;
	};

}