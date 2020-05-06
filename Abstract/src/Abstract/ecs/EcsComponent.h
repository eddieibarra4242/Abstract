#pragma once
#include <cstdint>
#include <vector>
#include <tuple>
#include "../core/Platform.h"
#include "../core/Debug.h"

namespace Abstract {

	struct ABSTRACT_API BaseECSComponent;

#define NULL_ENTITY_HANDLE nullptr
	typedef void* EntityHandle;
	typedef uint64_t(*ECSComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp);
	typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

	struct ABSTRACT_API BaseECSComponent
	{
	public:
		static uint64_t registerComponentType(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size);
		EntityHandle entity = NULL_ENTITY_HANDLE;

		inline static ECSComponentCreateFunction getTypeCreateFunction(uint64_t id)
		{
			return std::get<0>((*componentTypes)[id]);
		}

		inline static ECSComponentFreeFunction getTypeFreeFunction(uint64_t id)
		{
			return std::get<1>((*componentTypes)[id]);
		}

		inline static size_t getTypeSize(uint64_t id)
		{
			return std::get<2>((*componentTypes)[id]);
		}

		inline static bool isTypeValid(uint64_t id)
		{
			return id < componentTypes->size();
		}
	private:
		static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* componentTypes;
	};

	template<typename T>
	struct ABSTRACT_API ECSComponent : public BaseECSComponent
	{
		static const ECSComponentCreateFunction CREATE_FUNCTION;
		static const ECSComponentFreeFunction FREE_FUNCTION;

		static const uint64_t ID;
		static const size_t SIZE;
	};

#ifdef AB_BUILD_DLL
	template<typename Component>
	uint64_t ABSTRACT_API ECSComponentCreate(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp)
	{
		uint64_t index = memory.size();
		memory.resize(index + Component::SIZE);

		Component* component = new(&memory[index]) Component(*(Component*)comp);

		if (component != NULL)
		{
			component->entity = entity;
		}
		else
		{
			DEBUG_LOG("ECS", Debug::Priority::ERROR, "Not enough memory to copy compnent!");
		}

		return index;
	}

	template<typename Component>
	void ABSTRACT_API ECSComponentFree(BaseECSComponent* comp)
	{
		Component* component = (Component*)comp;
		component->~Component();
	}

	template<typename T>
	const uint64_t ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

	template<typename T>
	const size_t ECSComponent<T>::SIZE(sizeof(T));

	template<typename T>
	const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

	template<typename T>
	const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);
#else
	template<typename Component>
	uint64_t ABSTRACT_API ECSComponentCreate(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp);

	template<typename Component>
	void ABSTRACT_API ECSComponentFree(BaseECSComponent* comp);
#endif
}