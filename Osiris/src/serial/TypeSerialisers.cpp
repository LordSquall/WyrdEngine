#pragma once
#include "osrpch.h"

/* local includes  */
#include "serial/TypeSerialisers.h"

namespace glm
{
	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec2& vector) {
		jsonxx::Array arr;
		arr << vector.x << vector.y;
		obj << arr;
		return obj;
	}

	glm::vec2& operator<<(glm::vec2& vector, const jsonxx::Array& arr) {
		vector.x = (float)arr.get<jsonxx::Number>(0);
		vector.y = (float)arr.get<jsonxx::Number>(1);
		return vector;
	}

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector) {
		jsonxx::Array arr;
		arr << vector.x << vector.y << vector.z;
		obj << arr;
		return obj;
	}

	glm::vec3& operator<<(glm::vec3& vector, const jsonxx::Array& arr) {
		vector.x = (float)arr.get<jsonxx::Number>(0);
		vector.y = (float)arr.get<jsonxx::Number>(1);
		vector.z = (float)arr.get<jsonxx::Number>(2);
		return vector;
	}
}

namespace Osiris
{
	jsonxx::Object& operator<<(jsonxx::Object& obj, const Osiris::Color& color) {
		jsonxx::Array arr;
		arr << color.r << color.g << color.b << color.a;
		obj << arr;
		return obj;
	}

	Osiris::Color& operator<<(Osiris::Color& color, const jsonxx::Array& arr) {
		color.r = (float)arr.get<jsonxx::Number>(0);
		color.g = (float)arr.get<jsonxx::Number>(1);
		color.b = (float)arr.get<jsonxx::Number>(2);
		color.a = (float)arr.get<jsonxx::Number>(3);
		return color;
	}
}