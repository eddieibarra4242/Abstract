#pragma once
#include "../core/Platform.h"
#include "EcsComponent.h"
#include "../core/Time.h"
#include "../core/Debug.h"

namespace Abstract {

	class ABSTRACT_API BaseECSSystem
	{
	public:
		enum
		{
			FLAG_NECESSARY = 0,
			FLAG_OPTIONAL = 1
			//FLAG_PARTIAL = 2 //TODO: Support this! (Partial Flag makes sure that there is at least one partial component in the entity to update the system)
		};

		BaseECSSystem() { }

		virtual void preFrameUpdate(Delta delta) { }
		virtual void preFrameFixedUpdate(Delta delta) { }
		virtual void preFrameRender(Delta delta) { }

		virtual void onUpdate(Delta delta, BaseECSComponent** components) { }
		virtual void onFixedUpdate(Delta delta, BaseECSComponent** components) { }
		virtual void onRender(Delta delta, BaseECSComponent** components) { }

		virtual void postFrameUpdate(Delta delta) { }
		virtual void postFrameFixedUpdate(Delta delta) { }
		virtual void postFrameRender(Delta delta) { }

		const inline std::vector<uint64_t>& getComponentTypes()
		{
			return componentTypes;
		}

		const inline std::vector<uint64_t>& getComponentFlags()
		{
			return componentFlags;
		}

		bool isValid() const;
	protected:
		inline void addComponentType(uint64_t componentType, uint64_t componentFlag = FLAG_NECESSARY)
		{
			componentTypes.push_back(componentType);
			componentFlags.push_back(componentFlag);
		}
	private:
		std::vector<uint64_t> componentTypes;
		std::vector<uint64_t> componentFlags;
	};

	class ABSTRACT_API ECSSystemList
	{
	public:
		inline bool addSystem(BaseECSSystem& system)
		{
			bool ret = system.isValid();

			if (ret)
			{
				systems.push_back(&system);
			}
			else
			{
				DEBUG_WARN("ECS", "System found with only Optional Components, NOT ADDING!");
			}

			return ret;
		}
		inline size_t size()
		{
			return systems.size();
		}
		inline BaseECSSystem* operator[](uint64_t index)
		{
			return systems[index];
		}
		bool removeSystem(BaseECSSystem& system);
	private:
		std::vector<BaseECSSystem*> systems;
	};

}