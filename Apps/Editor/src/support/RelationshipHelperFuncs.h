#pragma once

#include "core/export.h"
#include "core/ecs/ECS.h"

namespace Wyrd
{
	class Scene;
}

namespace Wyrd::Editor
{
	class RelationshipHelperFuncs
	{
	public:
		static void AddChild(Wyrd::Scene* scene, Entity parent, Entity child);

		static void Remove(Wyrd::Scene* scene, Entity entity);
	};
}