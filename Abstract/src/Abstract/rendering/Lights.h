#pragma once
#include "../ecs/EcsComponent.h"
#include "../math/Vector.h"

#include "../core/Platform.h"

namespace Abstract {

	struct ABSTRACT_API DirectionalLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		float intensity;
	};

	//support ranges soon
	struct ABSTRACT_API PointLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		float intensity;
		Vector3 attenuation;
		float range;
	};

	struct ABSTRACT_API SpotLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		float intensity;
		Vector3 attenuation;
		float range;
		float angle;
	};

}