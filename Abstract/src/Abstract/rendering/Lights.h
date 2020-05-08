#pragma once
#include "../ecs/EcsComponent.h"
#include "../math/Vector.h"

#include "../core/Platform.h"

namespace Abstract {

	struct ABSTRACT_API DirectionalLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		double intensity = 0;
	};

	//support ranges soon
	struct ABSTRACT_API PointLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		double intensity = 0;
		Vector3 attenuation;
		double range = 0;
	};

	struct ABSTRACT_API SpotLight : public ECSComponent<DirectionalLight>
	{
		Vector3 color;
		double intensity = 0;
		Vector3 attenuation;
		double range = 0;
		double angle = 0;
	};

}