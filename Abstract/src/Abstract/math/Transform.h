#pragma once

#include "../ecs/EcsComponent.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace Abstract {

	class ABSTRACT_API Transform
	{
	public:
		Transform(const Vector3& positionIn = Vector3(), const Quaternion& rotationIn = Quaternion(), const Vector3& scaleIn = Vector3(1, 1, 1)) :
			position(positionIn), rotation(rotationIn), scale(scaleIn) { }
		~Transform() {}

		Matrix4 getModelMat();
		Matrix4 getViewMat();

		inline Vector3& getPosition() { return position; }
		inline Quaternion& getRotation() { return rotation; }
		inline Vector3& getScale() { return scale; }

		inline void setPosition(const Vector3& position) { this->position = position; }
		inline void setRotation(const Quaternion& rotation) { this->rotation = rotation; }
		inline void setScale(const Vector3& scale) { this->scale = scale; }

		inline void move(const Vector3& amt) { this->position = position + amt; }
		inline void rotate(const Vector3& axis, float angle) { rotation = (Quaternion(axis, angle) * rotation).normalized(); }
	private:
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};

	struct ABSTRACT_API TransformComp : ECSComponent<TransformComp>
	{
		Transform transform;
	};

}