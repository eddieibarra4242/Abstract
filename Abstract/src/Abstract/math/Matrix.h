#pragma once
#include "../core/Platform.h"
#include "Vector.h"
#include "Quaternion.h"

namespace Abstract {

	class ABSTRACT_API Matrix4
	{
	public:
		Matrix4() : m() { setIdentity(); }

		inline double get(int x, int y) const { return m[x][y]; }
		inline void set(double v, int x, int y) { this->m[x][y] = v; }

		Matrix4& setIdentity();

		Matrix4& setTranslation(Vector3 translation);
		Matrix4& setRotation(Quaternion rotation);
		Matrix4& setScale(Vector3 scale);

		Matrix4& setPerspective(double fov, double ar, double near, double far);

		inline const double* getData() const { return &m[0][0]; }

		inline Matrix4 operator*(const Matrix4& other) const
		{
			Matrix4 res;

			//this: rows, other: cols
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					res.m[x][y] = m[0][y] * other.m[x][0] +
						m[1][y] * other.m[x][1] +
						m[2][y] * other.m[x][2] +
						m[3][y] * other.m[x][3];
				}
			}

			return res;
		}

		~Matrix4() { }
	private:
		double m[4][4];
	};

}