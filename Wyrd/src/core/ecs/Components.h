#pragma once

#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/ResourcesUIDs.h"
#include "core/maths/Rect.h"
#include "core/ecs/ECS.h"
#include "core/behaviour/Properties/ScriptProperty.h"

#include <glm/glm.hpp>

#define METADATA_COMP_NAME_LENGTH		32
#define SCRIPT_COMP_PROP_MAX			32
#define SCRIPT_COMP_PROP_DATA_LENGTH	1024
#define TEXT_COMP_MAX_LENGTH			1024
#define TEXT_COMP_FONT_NAME_MAX_LENGTH  64

namespace Wyrd
{
	struct WYRD_LIBRARY_API MetaDataComponent
	{
		char name[METADATA_COMP_NAME_LENGTH];

		MetaDataComponent() : name("Entity") {}
	};

	struct WYRD_LIBRARY_API Transform2DComponent
	{
		glm::vec2 position;
		glm::vec2 rotationOrigin;
		float rotation;

		Transform2DComponent() : position(0.0f, 0.0f), rotationOrigin(0.0f, 0.0f), rotation(0.0f) {}

		glm::mat4 CalculateModelMatrix();
	};

	struct WYRD_LIBRARY_API SpriteComponent
	{
		glm::vec2 position;
		glm::vec2 size;
		glm::vec2 tiling;
		Color color;
		UID texture;

		SpriteComponent() : position(0.0f, 0.0f), size(64.0f, 64.0f), tiling(1.0f, 1.0f), color({ 1.0f, 1.0f, 1.0f, 1.0f }), texture(UID(RESOURCE_DEFAULT_TEXTURE)) {}
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

		CameraComponent() : viewport(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), aspectRatio(0.0f), size(0.0f) {}
	};

	struct WYRD_LIBRARY_API TextComponent
	{
		char content[TEXT_COMP_MAX_LENGTH];
		Color color;
		char font[TEXT_COMP_FONT_NAME_MAX_LENGTH];
		float size;

		TextComponent() : content("Text"), color({ 1.0f, 1.0f, 1.0f, 1.0f }), font("ProggyClean"), size(16.0f) {}
	};

	struct WYRD_LIBRARY_API RelationshipComponent
	{
		Entity first;
		Entity previous;
		Entity next;
		Entity parent;
		uint32_t childrenCnt;
		int32_t depth;
		bool remove;

		RelationshipComponent() : first(ENTITY_INVALID), previous(ENTITY_INVALID), next(ENTITY_INVALID), parent(ENTITY_INVALID), childrenCnt(0), depth(0), remove(false) {}
	};
};