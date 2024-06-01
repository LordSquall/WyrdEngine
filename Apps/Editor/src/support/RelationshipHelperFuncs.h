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
		enum class AddOp
		{
			Before, After, On
		};

	public:

		static bool CanAddChild(Wyrd::Scene* scene, Entity parent, Entity child, AddOp addOp);

		static void AddChild(Wyrd::Scene* scene, Entity parent, Entity child, AddOp addOp);

		static void ClearEntity(Wyrd::Scene* scene, Entity entity);

		static void Remove(Wyrd::Scene* scene, Entity entity);
	};
}