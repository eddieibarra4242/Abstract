#include "EcsComponent.h"

namespace Abstract {

	std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* BaseECSComponent::componentTypes;

	uint64_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size)
	{
		if (componentTypes == nullptr)
		{
			componentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>();
		}

		uint64_t componentID = componentTypes->size();

		componentTypes->push_back(std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>(createfn, freefn, size));

		return componentID;
	}

}