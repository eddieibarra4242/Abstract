#pragma once
#include "../ecs/EcsComponent.h"
#include "../math/Matrix.h"
#include "../core/Platform.h"

namespace Abstract {

	struct ABSTRACT_API Camera : public ECSComponent<Camera>
	{
		Matrix4 projection;
	};

}