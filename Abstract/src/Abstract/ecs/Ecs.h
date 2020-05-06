#pragma once

#include "../core/Platform.h"
#include "EcsComponent.h"
#include "EcsSystem.h"
#include "../core/Time.h"
#include "../core/Debug.h"
#include <map>

#define NULL_COPY_AND_ASSIGN(T) \
	T(const T& other) {(void)other;} \
	void operator=(const T& other) { (void)other; }

namespace Abstract {

	class ABSTRACT_API ECS
	{
	public:
		ECS() { }
		~ECS();

		enum class UpdateType
		{
			WHENEVER,
			FIXED,
			RENDER
		};

		//Entity Functions
		EntityHandle makeEntity(BaseECSComponent** entityComponents, const uint64_t* componentIDs, size_t numComponents);
		void removeEntity(EntityHandle handle);

		template<class A>
		EntityHandle makeEntity(A& c1)
		{
			BaseECSComponent* components[] = { &c1 };
			uint64_t componentIDs[] = { A::ID };
			return makeEntity(components, componentIDs, 1);
		}

		template<class A, class B>
		EntityHandle makeEntity(A& c1, B& c2)
		{
			BaseECSComponent* components[] = { &c1, &c2 };
			uint64_t componentIDs[] = { A::ID, B::ID };
			return makeEntity(components, componentIDs, 2);
		}

		template<class A, class B, class C>
		EntityHandle makeEntity(A& c1, B& c2, C& c3)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID };
			return makeEntity(components, componentIDs, 3);
		}

		template<class A, class B, class C, class D>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID };
			return makeEntity(components, componentIDs, 4);
		}

		template<class A, class B, class C, class D, class E>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID };
			return makeEntity(components, componentIDs, 5);
		}

		template<class A, class B, class C, class D, class E, class F>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID };
			return makeEntity(components, componentIDs, 6);
		}

		template<class A, class B, class C, class D, class E, class F, class G>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID };
			return makeEntity(components, componentIDs, 7);
		}

		template<class A, class B, class C, class D, class E, class F, class G, class H>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID };
			return makeEntity(components, componentIDs, 8);
		}

		template<class A, class B, class C, class D, class E, class F, class G, class H, class I>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID };
			return makeEntity(components, componentIDs, 9);
		}

		template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
		EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9, J& c10)
		{
			BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10 };
			uint64_t componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID, J::ID };
			return makeEntity(components, componentIDs, 10);
		}

		//Component Functions
		template<class Component>
		inline void addComponent(EntityHandle entity, Component* component)
		{
			addComponentInternal(entity, handleToEntity(entity), Component::ID, component);
		}

		template<class Component>
		inline bool removeComponent(EntityHandle entity)
		{
			return removeComponentInternal(entity, Component::ID);
		}

		template<class Component>
		inline Component* getComponent(EntityHandle entity)
		{
			return (Component*)getComponentInternal(handleToEntity(entity), components[Component::ID], Component::ID);
		}

		//System Components
		void updateSystems(ECSSystemList& systems, Delta delta, UpdateType updateType);
	private:
		std::map<uint64_t, std::vector<uint8_t>> components;
		std::vector<std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>*> entities;

		inline std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>* handleToRawType(EntityHandle handle)
		{
			return (std::pair<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>*)handle;
		}

		inline uint64_t handleToEntityIndex(EntityHandle handle)
		{
			return handleToRawType(handle)->first;
		}

		inline std::vector<std::pair<uint64_t, uint64_t>>& handleToEntity(EntityHandle handle)
		{
			return handleToRawType(handle)->second;
		}

		void deleteComponent(uint64_t componentID, uint64_t index);
		bool removeComponentInternal(EntityHandle handle, uint64_t componentID);
		void addComponentInternal(EntityHandle handle, std::vector<std::pair<uint64_t, uint64_t>>& entity, uint64_t componentID, BaseECSComponent* component);
		BaseECSComponent* getComponentInternal(std::vector<std::pair<uint64_t, uint64_t>>& entity, std::vector<uint8_t>& array, uint64_t componentID);

		void updateSystemWithMultipleComponents(ECSSystemList& systems, uint64_t index, Delta delta, const std::vector<uint64_t>& componentTypes,
			std::vector<BaseECSComponent*>& componentParam, std::vector<std::vector<uint8_t>*>& componentArrays, UpdateType updateType);
		uint64_t findLeastCommonComponent(const std::vector<uint64_t>& componentTypes, const std::vector<uint64_t>& componentFlags);

		inline void callSystemPreFrameInternal(Delta delta, BaseECSSystem* system, UpdateType ut)
		{
			switch (ut)
			{
			case UpdateType::WHENEVER: system->preFrameUpdate(delta);      return;
			case UpdateType::FIXED:    system->preFrameFixedUpdate(delta); return;
			case UpdateType::RENDER:   system->preFrameRender(delta);      return;
			}
		}

		inline void callSystemUpdateInternal(Delta delta, BaseECSComponent** components, BaseECSSystem* system, UpdateType ut)
		{
			switch (ut)
			{
			case UpdateType::WHENEVER: system->onUpdate(delta, components);      return;
			case UpdateType::FIXED:    system->onFixedUpdate(delta, components); return;
			case UpdateType::RENDER:   system->onRender(delta, components);      return;
			}
		}

		inline void callSystemPostFrameInternal(Delta delta, BaseECSSystem* system, UpdateType ut)
		{
			switch (ut)
			{
			case UpdateType::WHENEVER: system->postFrameUpdate(delta);      return;
			case UpdateType::FIXED:    system->postFrameFixedUpdate(delta); return;
			case UpdateType::RENDER:   system->postFrameRender(delta);      return;
			}
		}

		NULL_COPY_AND_ASSIGN(ECS);
	};

}