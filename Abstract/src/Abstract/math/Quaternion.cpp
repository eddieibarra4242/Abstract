#include "Quaternion.h"

namespace Abstract {

	Quaternion::Quaternion(Vector3 axis, float angle)
	{
		float sin = sinf(angle / 2.0f);
		float cos = cosf(angle / 2.0f);

		this->x = axis.getX() * sin;
		this->y = axis.getY() * sin;
		this->z = axis.getZ() * sin;
		this->w = cos;

		float len = length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
		this->w /= len;
	}

}