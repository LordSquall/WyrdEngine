#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/ecs/System.h"

namespace Wyrd 
{
	class WYRD_LIBRARY_API ECS
	{
	public:
		virtual ~ECS() {};

		template <typename Component>
		void RegisterSystem(System<Component>* system);

	};
};