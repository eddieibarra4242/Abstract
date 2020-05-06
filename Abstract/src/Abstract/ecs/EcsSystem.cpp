#include "EcsSystem.h"

namespace Abstract {

	bool BaseECSSystem::isValid() const
	{
		for (uint32_t i = 0; i < componentFlags.size(); i++)
		{
			if (componentFlags[i] != BaseECSSystem::FLAG_OPTIONAL)
			{
				return true;
			}
		}

		return false;
	}

	bool ECSSystemList::removeSystem(BaseECSSystem& system)
	{
		for (uint32_t i = 0; i < systems.size(); i++)
		{
			if (&system == systems[i])
			{
				systems.erase(systems.begin() + i);
				return true;
			}
		}
		return false;
	}

}