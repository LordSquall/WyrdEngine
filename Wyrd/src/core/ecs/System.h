#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/ecs/Entity.h"

namespace Wyrd 
{
	template<typename Component>
	class WYRD_LIBRARY_API System
	{
	public:
		virtual ~System() {};

		Component& AddComponent(Entity entity)
		{
			_Entities.push_back(entity);

			return _Entities.back();
		}

		virtual void Update() = 0;

	private:
		std::vector<Entity> _Entities;
		std::vector<Component> _Components;
	};
};