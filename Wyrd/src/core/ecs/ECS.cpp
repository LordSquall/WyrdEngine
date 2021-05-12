#include "wyrdpch.h"
#include "core/Log.h"
#include "core/ecs/ECS.h"

namespace Wyrd
{
	template <typename Component>
	void ECS::RegisterSystem(System<Component>* system)
	{
		WYRD_CORE_TRACE("System Added");
	}
};