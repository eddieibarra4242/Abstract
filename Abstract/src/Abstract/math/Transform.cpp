#include "Transform.h"

namespace Abstract {

	Matrix4 Transform::getModelMat()
	{
		Matrix4 posMat = Matrix4().setTranslation(position);
		Matrix4 rotMat = Matrix4().setRotation(rotation);
		Matrix4 scaleMat = Matrix4().setScale(scale);

		return posMat * rotMat * scaleMat;
	}

	Matrix4 Transform::getViewMat()
	{
		Matrix4 posMat = Matrix4().setTranslation(-position);
		Matrix4 rotMat = Matrix4().setRotation(rotation.conjugate());

		return posMat * rotMat;
	}
}