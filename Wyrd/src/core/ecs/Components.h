#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/maths/Rect.h"
#include "core/ecs/ECS.h"
#include "core/behaviour/Properties/ScriptProperty.h"

#include <glm/glm.hpp>

#define METADATA_COMP_NAME_LENGTH		32
#define SCRIPT_COMP_PROP_MAX			32
#define SCRIPT_COMP_PROP_DATA_LENGTH	1024

namespace Wyrd
{
	struct WYRD_LIBRARY_API MetaDataComponent
	{
		char name[METADATA_COMP_NAME_LENGTH];
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
		int32_t instanceID;
		int32_t propertyCount;
		char properties[SCRIPT_COMP_PROP_MAX][SCRIPT_COMP_PROP_DATA_LENGTH];
		std::map<std::string, void*> propertyMap;

		ScriptComponent() : script(UID()), instanceID(0), propertyCount(0)
		{
			for (int i = 0; i < SCRIPT_COMP_PROP_MAX; i++)
			{
				properties[i][0] = '\0';
			}
		}
	};

	struct WYRD_LIBRARY_API CameraComponent
	{
		Rect viewport;
		float aspectRatio;
		float size;
	};
};