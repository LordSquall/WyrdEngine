#pragma once

#include "wyrdpch.h"
#include "core/export.h"

namespace Wyrd
{
	struct WYRD_LIBRARY_API MetaDataComponent
	{
		std::string name;
		std::string layerName;
	};

	struct WYRD_LIBRARY_API LayerComponent
	{
		bool enabled;
	};

	struct WYRD_LIBRARY_API ECSTransform2DComponent
	{
		glm::vec2 position;
		float rotation;
	};

	struct WYRD_LIBRARY_API ECSSpriteComponent
	{
		glm::vec2 position;
		glm::vec2 size;
	};
};