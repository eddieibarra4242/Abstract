#pragma once

#include "../ecs/EcsComponent.h"
#include "Texture.h"
#include "../core/Platform.h"

namespace Abstract {

	struct ABSTRACT_API Material : public ECSComponent<Material>
	{
		Texture texture;
		double specIntensity = 0;
		double specPower = 0;
	};

}