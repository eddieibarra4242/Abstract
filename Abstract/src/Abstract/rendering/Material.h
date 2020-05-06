#pragma once

#include "../ecs/EcsComponent.h"
#include "Texture.h"
#include "../core/Platform.h"

namespace Abstract {

	struct ABSTRACT_API Material : public ECSComponent<Material>
	{
		Texture texture;
		float specIntensity;
		float specPower;
	};

}