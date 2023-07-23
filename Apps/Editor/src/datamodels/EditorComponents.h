#pragma once

/* core includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/maths/BoundingBox.h>
#include <core/ecs/ComponentPool.h>

namespace Wyrd::Editor
{
	struct WYRD_LIBRARY_API EditorComponent
	{
		BoundingBox inputBoundingBox;

		static void ResetFunc(ComponentPool* pool, Entity entity) {};
	};
};