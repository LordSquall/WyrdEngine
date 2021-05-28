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

namespace Wyrd
{
	jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Color& color);

	Wyrd::Color& operator<<(Wyrd::Color& color, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Vector2& vector);

	Wyrd::Vector2& operator<<(Wyrd::Vector2& vector, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Vector3& vector);

	Wyrd::Vector3& operator<<(Wyrd::Vector3& vector, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::UID& uid);

	Wyrd::UID& operator<<(Wyrd::UID& uid, const jsonxx::Array& arr);
}