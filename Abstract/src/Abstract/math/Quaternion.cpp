#include "Quaternion.h"

namespace Abstract {

	Quaternion::Quaternion(Vector3 axis, double angle)
	{
		double sin = std::sin(angle / 2.0f);
		double cos = std::cos(angle / 2.0f);

		this->x = axis.getX() * sin;
		this->y = axis.getY() * sin;
		this->z = axis.getZ() * sin;
		this->w = cos;

		double len = length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
		this->w /= len;
	}

}