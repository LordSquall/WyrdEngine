#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/maths/Rect.h"
#include "core/ecs/ECS.h"

#include <jsonxx.h>
#include <glm/glm.hpp>

using namespace jsonxx;

namespace Wyrd
{
	struct WYRD_LIBRARY_API MetaDataComponent
	{
		char name[32];
	};

	struct WYRD_LIBRARY_API Transform2DComponent
	{
		glm::vec2 position;
		float rotation;
	};

	struct WYRD_LIBRARY_API SpriteComponent
	{
		glm::vec2 position;
		glm::vec2 size;
		Color color;
		UID texture;
	};

	/* Script Components */
	struct WYRD_LIBRARY_API ScriptComponent
	{
		UID script;
		uint32_t instanceID;
	};

	struct WYRD_LIBRARY_API CameraComponent
	{
		float size;
	};

	struct WYRD_LIBRARY_API EditorComponent
	{
		Rect inputArea;
	};
};