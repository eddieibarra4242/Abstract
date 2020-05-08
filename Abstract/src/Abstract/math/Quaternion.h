#pragma once
#include "../core/Platform.h"
#include "Vector.h"

namespace Abstract {

	class ABSTRACT_API Quaternion
	{
	public:
		Quaternion(double x = 0.0f, double y = 0.0f, double z = 0.0f, double w = 1.0f) : x(x), y(y), z(z), w(w) { }
		Quaternion(Vector3 axis, double angle);

		inline double getX() const { return x; }
		inline double getY() const { return y; }
		inline double getZ() const { return z; }
		inline double getW() const { return w; }

		inline double length() const { return sqrt(x * x + y * y + z * z + w * w); }
		inline Quaternion normalized() const { return this->operator/(length()); }

		inline Quaternion conjugate() const
		{
			return Quaternion(-x, -y, -z, w);
		}

		inline Vector3 getMathRight()   const { return Vector3(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y)); }
		inline Vector3 getMathUp()      const { return Vector3(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x)); }
		inline Vector3 getMathForward() const { return Vector3(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y)); }

		inline Vector3 getRight()	const { return rotate(Vector3(1, 0, 0)).normalized(); }
		inline Vector3 getLeft()	const { return rotate(Vector3(-1, 0, 0)).normalized(); }
		inline Vector3 getUp()		const { return rotate(Vector3(0, 1, 0)).normalized(); }
		inline Vector3 getDown()	const { return rotate(Vector3(0, -1, 0)).normalized(); }
		inline Vector3 getForward() const { return rotate(Vector3(0, 0, 1)).normalized(); }
		inline Vector3 getBack()    const { return rotate(Vector3(0, 0, -1)).normalized(); }

#pragma region Operator Overloads

		inline Quaternion operator-() const
		{
			return Quaternion(-x, -y, -z, w);
		}

		inline Quaternion operator+(const double& other) const
		{
			return Quaternion(x + other, y + other, z + other, w + other);
		}

		inline Quaternion operator+(const Quaternion& other) const
		{
			return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline Quaternion operator-(const double& other) const
		{
			return Quaternion(x - other, y - other, z - other, w - other);
		}

		inline Quaternion operator-(const Quaternion& other) const
		{
			return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline Quaternion operator*(const Quaternion& other) const
		{
			double w_ = w * other.w - x * other.x - y * other.y - z * other.z;
			double x_ = x * other.w + w * other.x + y * other.z - z * other.y;
			double y_ = y * other.w + w * other.y + z * other.x - x * other.z;
			double z_ = z * other.w + w * other.z + x * other.y - y * other.x;

			return Quaternion(x_, y_, z_, w_);
		}

		inline Quaternion operator*(const Vector3& other) const
		{
			double w_ = -x * other.getX() - y * other.getY() - z * other.getZ();
			double x_ = w * other.getX() + y * other.getZ() - z * other.getY();
			double y_ = w * other.getY() + z * other.getX() - x * other.getZ();
			double z_ = w * other.getZ() + x * other.getY() - y * other.getX();

			return Quaternion(x_, y_, z_, w_);
		}

		inline Vector3 rotate(const Vector3& other) const
		{
			Quaternion r = this->operator*(other) *conjugate();

			return Vector3(r.getX(), r.getY(), r.getZ());
		}

		inline Quaternion operator/(const double& other) const
		{
			return Quaternion(x / other, y / other, z / other, w / other);
		}

		constexpr void operator+=(const double& other)
		{
			this->x += other;
			this->y += other;
			this->z += other;
			this->w += other;
		}

		constexpr void operator+=(const Quaternion& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
		}

		constexpr void operator-=(const double& other)
		{
			this->x -= other;
			this->y -= other;
			this->z -= other;
			this->w -= other;
		}

		constexpr void operator-=(const Quaternion& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
		}

#pragma endregion

		~Quaternion() { }
	private:
		double x;
		double y;
		double z;
		double w;
	};

}