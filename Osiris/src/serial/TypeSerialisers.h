#pragma once

/* local includes  */
#include "core/Structures.h"

/* external includes */
#include <jsonxx.h>
#include <glm/glm.hpp>

namespace glm
{

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector);

	glm::vec2& operator<<(glm::vec2& vector, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec2& vector);
	
	glm::vec3& operator<<(glm::vec3& vector, const jsonxx::Array& arr);
}

namespace Osiris
{
	jsonxx::Object& operator<<(jsonxx::Object& obj, const Osiris::Color& color);

	Osiris::Color& operator<<(Osiris::Color& color, const jsonxx::Array& arr);
}