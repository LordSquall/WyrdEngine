#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/UID.h"
#include "core/maths/Rect.h"
#include "core/renderer/Texture.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace glm
{
	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector);

	glm::vec2& operator<<(glm::vec2& vector, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector);

	glm::vec3& operator<<(glm::vec3& vector, const jsonxx::Array& arr);

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector);

	glm::vec4& operator<<(glm::vec4& vector, const jsonxx::Array& arr);
}

namespace Wyrd
{
	void writeStr(std::ofstream& os, const std::string& str);
	std::string readStr(std::ifstream& is);
	void writeGuid(std::ofstream& os, const Wyrd::UID& guid);
	Wyrd::UID readGuid(std::ifstream& is);

	jsonxx::String DecodeString(const std::string& encodedData = "");
	jsonxx::Object DecodeVec2(const std::string& encodedData = "");
	jsonxx::Object DecodeVec3(const std::string& encodedData = "");
	jsonxx::Object DecodeVec4(const std::string& encodedData = "");
	jsonxx::Object DecodeColor(const std::string& encodedData = "");
	jsonxx::Object DecodeRect(const std::string& encodedData = "");

	char* operator<<(char* data, const jsonxx::String val);

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector2& val);
	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector3& val);
	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector4& val);
	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Color& val);
	jsonxx::Object& operator<<(jsonxx::Object& data, Wyrd::Texture** val);
	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::UID& val);
	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Rect& val);

	Wyrd::Vector2& operator<<(Wyrd::Vector2& data, const jsonxx::Object& val);
	Wyrd::Vector3& operator<<(Wyrd::Vector3& data, const jsonxx::Object& val);
	Wyrd::Vector4& operator<<(Wyrd::Vector4& data, const jsonxx::Object& val);
	Wyrd::Color& operator<<(Wyrd::Color& data, const jsonxx::Object& val);
	Wyrd::Texture** operator<<(Wyrd::Texture** data, const jsonxx::String val);
	Wyrd::UID& operator<<(Wyrd::UID& data, const jsonxx::String val);
	Wyrd::Rect& operator<<(Wyrd::Rect& data, const jsonxx::Object& val);
}