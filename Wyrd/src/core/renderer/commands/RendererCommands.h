#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"

/* external includes */
#include <string>
#include <glm/glm.hpp>

namespace Wyrd
{
	class Shader;
	class Texture;

	struct WYRD_LIBRARY_API DrawCommand
	{
		uint32_t type;
		glm::mat4 vpMatrix;
		Shader* shader;
	};

	struct WYRD_LIBRARY_API DrawSpriteCommand : public DrawCommand
	{
		glm::vec2 position;
		glm::vec2 size;
		Texture* texture;
		Color color;
	};
}