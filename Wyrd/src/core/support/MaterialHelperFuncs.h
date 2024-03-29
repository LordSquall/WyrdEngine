#pragma once

#include "core/export.h"
#include "core/ecs/ECS.h"

namespace Wyrd
{
	class Scene;
}

namespace Wyrd
{
	class MaterialHelperFuncs
	{
	public:

		static void AssignToComponent(MaterialComponent* materialComponent);
	};
}