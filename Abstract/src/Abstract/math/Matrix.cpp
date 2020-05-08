#include "Matrix.h"

namespace Abstract {

	Matrix4& Matrix4::setIdentity()
	{
		m[0][0] = 1; m[1][0] = 0; m[2][0] = 0; m[3][0] = 0;
		m[0][1] = 0; m[1][1] = 1; m[2][1] = 0; m[3][1] = 0;
		m[0][2] = 0; m[1][2] = 0; m[2][2] = 1; m[3][2] = 0;
		m[0][3] = 0; m[1][3] = 0; m[2][3] = 0; m[3][3] = 1;

		return (*this);
	}

	Matrix4& Matrix4::setTranslation(Vector3 translation)
	{
		m[0][0] = 1; m[1][0] = 0; m[2][0] = 0; m[3][0] = translation.getX();
		m[0][1] = 0; m[1][1] = 1; m[2][1] = 0; m[3][1] = translation.getY();
		m[0][2] = 0; m[1][2] = 0; m[2][2] = 1; m[3][2] = translation.getZ();
		m[0][3] = 0; m[1][3] = 0; m[2][3] = 0; m[3][3] = 1;

		return (*this);
	}

	Matrix4& Matrix4::setRotation(Quaternion rotation)
	{
		Vector3 r = rotation.getMathRight();
		Vector3 u = rotation.getMathUp();
		Vector3 f = rotation.getMathForward();

		m[0][0] = r.getX(); m[1][0] = r.getY(); m[2][0] = r.getZ(); m[3][0] = 0;
		m[0][1] = u.getX(); m[1][1] = u.getY(); m[2][1] = u.getZ(); m[3][1] = 0;
		m[0][2] = f.getX(); m[1][2] = f.getY(); m[2][2] = f.getZ(); m[3][2] = 0;
		m[0][3] = 0; m[1][3] = 0; m[2][3] = 0; m[3][3] = 1;

		return (*this);
	}

	Matrix4& Matrix4::setScale(Vector3 scale)
	{
		m[0][0] = scale.getX(); m[1][0] = 0; m[2][0] = 0; m[3][0] = 0;
		m[0][1] = 0; m[1][1] = scale.getY(); m[2][1] = 0; m[3][1] = 0;
		m[0][2] = 0; m[1][2] = 0; m[2][2] = scale.getZ(); m[3][2] = 0;
		m[0][3] = 0; m[1][3] = 0; m[2][3] = 0; m[3][3] = 1;

		return (*this);
	}

	Matrix4& Matrix4::setPerspective(double fov, double ar, double near, double far)
	{
		double tanHalfFov = tanf(fov / 2.0f);
		double zRange = near - far;

		m[0][0] = 1.0 / (tanHalfFov * ar);  m[1][0] = 0;				 m[2][0] = 0;					   m[3][0] = 0;
		m[0][1] = 0;						m[1][1] = 1.0 / tanHalfFov;  m[2][1] = 0;					   m[3][1] = 0;
		m[0][2] = 0;						m[1][2] = 0;				 m[2][2] = (-near - far) / zRange; m[3][2] = 2 * far * near / zRange;
		m[0][3] = 0;						m[1][3] = 0;				 m[2][3] = 1;					   m[3][3] = 0;

		return (*this);
	}

}