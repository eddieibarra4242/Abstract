#include "Ecs.h"
#include <cmath>

namespace Abstract {

	ECS::~ECS()
	{
		for (std::map<uint64_t, std::vector<uint8_t>>::iterator it = components.begin(); it != components.end(); ++it)
		{
			size_t typeSize = BaseECSComponent::getTypeSize(it->first);
			ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(it->first);
			for (uint64_t i = 0; i < it->second.size(); i += typeSize)
			{
				freefn((BaseECSComponent*)&it->second[i]);
			}
		}

		for (uint64_t i = 0; i < entities.size(); i++)
		{
			delete entities[i];
		}
	}

	EntityHandle ECS::makeEntity(BaseECSComponent** entityComponents, const uint64_t* componentIDs, size_t numComponents)
	{
		std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>* newEntity = new std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>();
		EntityHandle handle = (EntityHandle)newEntity;

		for (uint64_t i = 0; i < numComponents; i++)
		{
			if (!BaseECSComponent::isTypeValid(componentIDs[i]))
			{
				DEBUG_LOG("ECS", Debug::Priority::ERROR, "'%u' is not a valid component type", componentIDs[i]);
				delete newEntity;
				return NULL_ENTITY_HANDLE;
			}

			addComponentInternal(handle, newEntity->second, componentIDs[i], entityComponents[i]);
		}

		newEntity->first = entities.size();
		entities.push_back(newEntity);

		return handle;
	}

	void ECS::removeEntity(EntityHandle handle)
	{
		std::vector<std::pair<uint64_t, uint64_t>>& entity = handleToEntity(handle);

		for (uint64_t i = 0; i < entity.size(); i++)
		{
			deleteComponent(entity[i].first, entity[i].second);
		}

		uint64_t destIndex = handleToEntityIndex(handle);
		uint64_t srcIndex = entities.size() - 1;
		delete entities[destIndex];
		entities[destIndex]->first = destIndex;
		entities.pop_back();
	}

	void ECS::deleteComponent(uint64_t componentID, uint64_t index)
	{
		std::vector<uint8_t>& array = components[componentID];
		ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(componentID);
		size_t typeSize = BaseECSComponent::getTypeSize(componentID);

		uint64_t srcIndex = array.size() - typeSize;
		BaseECSComponent* srcComponent = (BaseECSComponent*)&array[srcIndex];
		BaseECSComponent* destComponent = (BaseECSComponent*)&array[index];
		freefn(destComponent);

		if (index == srcIndex)
		{
			array.resize(srcIndex);
			return;
		}

		memcpy(destComponent, srcComponent, typeSize);

		std::vector<std::pair<uint64_t, uint64_t>>& srcEntity = handleToEntity(srcComponent->entity);
		for (uint64_t i = 0; i < srcEntity.size(); i++)
		{
			if (componentID == srcEntity[i].first && srcIndex == srcEntity[i].second)
			{
				srcEntity[i].second = index;
				break;
			}
		}

		array.resize(srcIndex);
	}

	bool ECS::removeComponentInternal(EntityHandle handle, uint64_t componentID)
	{
		std::vector<std::pair<uint64_t, uint64_t>>& entity = handleToEntity(handle);
		for (uint64_t i = 0; i < entity.size(); i++)
		{
			if (componentID == entity[i].first)
			{
				deleteComponent(entity[i].first, entity[i].second);
				uint64_t srcIndex = entity.size() - 1;
				uint64_t destIndex = i;
				entity[destIndex] = entity[srcIndex];
				entity.pop_back();
				return true;
			}
		}

		return false;
	}

	void ECS::addComponentInternal(EntityHandle handle, std::vector<std::pair<uint64_t, uint64_t>>& entity, uint64_t componentID, BaseECSComponent* component)
	{
		ECSComponentCreateFunction createfn = BaseECSComponent::getTypeCreateFunction(componentID);
		std::pair<uint64_t, uint64_t> newPair;
		newPair.first = componentID;
		newPair.second = createfn(this->components[componentID], handle, component);
		entity.push_back(newPair);
	}

	BaseECSComponent* ECS::getComponentInternal(std::vector<std::pair<uint64_t, uint64_t>>& entity, std::vector<uint8_t>& array, uint64_t componentID)
	{
		for (uint64_t i = 0; i < entity.size(); i++)
		{
			if (componentID == entity[i].first)
			{
				return (BaseECSComponent*)&array[entity[i].second];
			}
		}

		return nullptr;
	}

	void ECS::updateSystems(ECSSystemList& systems, Delta delta, UpdateType updateType)
	{
		std::vector<BaseECSComponent*> componentParam;
		std::vector<std::vector<uint8_t>*> componentArrays;

		for (uint64_t i = 0; i < systems.size(); i++)
		{
			callSystemPreFrameInternal(delta, systems[i], updateType);

			const std::vector<uint64_t>& componentTypes = systems[i]->getComponentTypes();

			if (componentTypes.size() == 1)
			{
				size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[0]);
				std::vector<uint8_t>& array = components[componentTypes[0]];

				for (uint64_t j = 0; j < array.size(); j += typeSize)
				{
					BaseECSComponent* component = (BaseECSComponent*)&array[j];
					callSystemUpdateInternal(delta, &component, systems[i], updateType);
				}
			}
			else
			{
				updateSystemWithMultipleComponents(systems, i, delta, componentTypes, componentParam, componentArrays, updateType);
			}

			callSystemPostFrameInternal(delta, systems[i], updateType);
		}
	}

	uint64_t ECS::findLeastCommonComponent(const std::vector<uint64_t>& componentTypes, const std::vector<uint64_t>& componentFlags)
	{
		uint64_t minSize = (uint64_t)-1;
		uint64_t minIndex = (uint64_t)-1;

		for (uint64_t i = 0; i < componentTypes.size(); i++)
		{
			if (componentFlags[i] == BaseECSSystem::FLAG_OPTIONAL)
			{
				continue;
			}

			size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[i]);
			uint64_t size = components[componentTypes[i]].size() / typeSize;
			if (size <= minSize)
			{
				minSize = size;
				minIndex = i;
			}
		}

		return minIndex;
	}

	void ECS::updateSystemWithMultipleComponents(ECSSystemList& systems, uint64_t index, Delta delta, const std::vector<uint64_t>& componentTypes,
		std::vector<BaseECSComponent*>& componentParam, std::vector<std::vector<uint8_t>*>& componentArrays, UpdateType updateType)
	{
		const std::vector<uint64_t>& componentFlags = systems[index]->getComponentFlags();

		componentParam.resize((uint64_t)std::fmax(componentParam.size(), componentTypes.size()));
		componentArrays.resize((uint64_t)std::fmax(componentArrays.size(), componentTypes.size()));

		for (uint64_t i = 0; i < componentTypes.size(); i++)
		{
			componentArrays[i] = &components[componentTypes[i]];
		}

		uint64_t minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);
		size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[minSizeIndex]);
		std::vector<uint8_t>& array = *componentArrays[minSizeIndex];
		for (uint64_t i = 0; i < array.size(); i += typeSize)
		{
			componentParam[minSizeIndex] = (BaseECSComponent*)&array[i];
			std::vector<std::pair<uint64_t, uint64_t>>& entity = handleToEntity(componentParam[minSizeIndex]->entity);

			bool isValid = true;
			for (uint64_t j = 0; j < componentTypes.size(); j++)
			{
				if (j == minSizeIndex)
				{
					continue;
				}

				componentParam[j] = getComponentInternal(entity, *componentArrays[j], componentTypes[j]);
				if (componentParam[j] == nullptr && componentFlags[j] != BaseECSSystem::FLAG_OPTIONAL)
				{
					isValid = false;
					break;
				}
			}

			if (isValid)
			{
				callSystemUpdateInternal(delta, (BaseECSComponent**)&componentParam[0], systems[index], updateType);
			}
		}
	}

}