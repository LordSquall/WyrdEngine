#include "wyrdpch.h"

/* local includes  */
#include "core/UID.h"
#include "core/Resources.h"
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

	jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec4& vector) {
		jsonxx::Array arr;
		arr << vector.x << vector.y << vector.z << vector.w;
		obj << arr;
		return obj;
	}

	glm::vec4& operator<<(glm::vec4& vector, const jsonxx::Array& arr) {
		vector.x = (float)arr.get<jsonxx::Number>(0);
		vector.y = (float)arr.get<jsonxx::Number>(1);
		vector.z = (float)arr.get<jsonxx::Number>(2);
		vector.w = (float)arr.get<jsonxx::Number>(3);
		return vector;
	}
}

namespace Wyrd
{

	void writeStr(std::ofstream& os, const std::string& str)
	{
		size_t size = str.size();
		os.write((char*)&size, sizeof(size_t));
		if (size > 0)
		{
			os.write((char*)str.c_str(), sizeof(unsigned char) * size);
		}
	}

	std::string readStr(std::ifstream& is)
	{
		size_t sz;
		is.read((char*)&sz, sizeof(size_t));

		if (sz > 0)
		{
			std::vector<char> buffer(sz);
			is.read(&buffer[0], buffer.size());
			std::string str(buffer.begin(), buffer.end());
			return str;
		}
		return "";
	}

	void writeGuid(std::ofstream& os, const Wyrd::UID& guid)
	{
		writeStr(os, guid.str());
	}

	Wyrd::UID readGuid(std::ifstream& is)
	{
		return Wyrd::UID(readStr(is));
	}

	const std::vector<float> ExplodeToFloats(const std::string& data)
	{
		std::vector<float> values;

		std::stringstream ss(data);

		for (float i; ss >> i;) {
			values.push_back(i);
			if (ss.peek() == ',')
				ss.ignore();
		}

		return values;
	}

	jsonxx::String DecodeString(const std::string& encodedData /* = "" */)
	{
		return jsonxx::String(encodedData);
	}

	jsonxx::Object DecodeVec2(const std::string& encodedData /* = "" */)
	{
		jsonxx::Object obj;

		if (encodedData != "")
		{
			std::vector<float> components = ExplodeToFloats(encodedData);

			obj << "x" << components[0];
			obj << "y" << components[1];
		}
		else
		{
			obj << "x" << 0.0f;
			obj << "y" << 0.0f;
		}
		return obj;
	}

	jsonxx::Object DecodeVec3(const std::string& encodedData /* = "" */)
	{
		jsonxx::Object obj;

		if (encodedData != "")
		{
			std::vector<float> components = ExplodeToFloats(encodedData);

			obj << "x" << components[0];
			obj << "y" << components[1];
			obj << "z" << components[2];
		}
		else
		{
			obj << "x" << 0.0f;
			obj << "y" << 0.0f;
			obj << "z" << 0.0f;
		}
		return obj;
	}

	jsonxx::Object DecodeVec4(const std::string& encodedData /* = "" */)
	{
		jsonxx::Object obj;

		if (encodedData != "")
		{
			std::vector<float> components = ExplodeToFloats(encodedData);

			obj << "x" << components[0];
			obj << "y" << components[1];
			obj << "z" << components[2];
			obj << "w" << components[3];
		}
		else
		{
			obj << "x" << 0.0f;
			obj << "y" << 0.0f;
			obj << "z" << 0.0f;
			obj << "w" << 0.0f;
		}
		return obj;
	}

	jsonxx::Object DecodeColor(const std::string& encodedData /* = "" */)
	{
		jsonxx::Object obj;

		if (encodedData != "")
		{
			std::vector<float> channels = ExplodeToFloats(encodedData);

			obj << "r" << channels[0];
			obj << "g" << channels[1];
			obj << "b" << channels[2];
			channels.size() == 4 ? obj << "a" << channels[3] : obj << "a" << 1.0f;
		}
		else
		{
			obj << "r" << 1.0f;
			obj << "g" << 1.0f;
			obj << "b" << 1.0f;
			obj << "a" << 1.0f;
		}
		return obj;
	}

	jsonxx::Object DecodeRect(const std::string& encodedData /* = "" */)
	{
		jsonxx::Object obj;

		if (encodedData != "")
		{
			std::vector<float> channels = ExplodeToFloats(encodedData);

			obj << "x" << channels[0];
			obj << "y" << channels[1];
			obj << "w" << channels[2];
			obj << "h" << channels[3];
		}
		else
		{
			obj << "x" << 0.0f;
			obj << "y" << 0.0f;
			obj << "w" << 0.0f;
			obj << "h" << 0.0f;
		}
		return obj;
	}

	char* operator<<(char* data, const jsonxx::String val)
	{
		strcpy(data, val.c_str());
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector2& val) {
		jsonxx::Object o;
		o << "x" << val.x;
		o << "y" << val.y;
		data << o;
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector3& val) {
		jsonxx::Object o;
		o << "x" << val.x;
		o << "y" << val.y;
		o << "z" << val.z;
		data << o;
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Vector4& val) {
		jsonxx::Object o;
		o << "x" << val.x;
		o << "y" << val.y;
		o << "z" << val.z;
		o << "w" << val.w;
		data << o;
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Color& val) {
		jsonxx::Object o;
		o << "r" << val.r;
		o << "g" << val.g;
		o << "b" << val.b;
		o << "a" << val.a;
		data << o;
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, Wyrd::Texture** val)
	{
		data << (*val)->GetUID();
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const UID& val) {
		data << val.str();
		return data;
	}

	jsonxx::Object& operator<<(jsonxx::Object& data, const Wyrd::Rect& val) {
		jsonxx::Object o;
		o << "x" << val._position.x;
		o << "y" << val._position.y;
		o << "w" << val._size.x;
		o << "h" << val._size.y;
		data << o;
		return data;
	}

	Wyrd::Vector2& operator<<(Wyrd::Vector2& data, const jsonxx::Object& val) {
		data.x = (float)val.get<jsonxx::Number>("x");
		data.y = (float)val.get<jsonxx::Number>("y");
		return data;
	}

	Wyrd::Vector3& operator<<(Wyrd::Vector3& data, const jsonxx::Object& val) {
		data.x = (float)val.get<jsonxx::Number>("x");
		data.y = (float)val.get<jsonxx::Number>("y");
		data.z = (float)val.get<jsonxx::Number>("z");
		return data;
	}

	Wyrd::Vector4& operator<<(Wyrd::Vector4& data, const jsonxx::Object& val) {
		data.x = (float)val.get<jsonxx::Number>("x");
		data.y = (float)val.get<jsonxx::Number>("y");
		data.z = (float)val.get<jsonxx::Number>("z");
		data.z = (float)val.get<jsonxx::Number>("w");
		return data;
	}

	Wyrd::Color& operator<<(Wyrd::Color& data, const jsonxx::Object& val) {
		data.r = (float)val.get<jsonxx::Number>("r");
		data.g = (float)val.get<jsonxx::Number>("g");
		data.b = (float)val.get<jsonxx::Number>("b");
		data.a = (float)val.get<jsonxx::Number>("a");
		return data;
	}

	Wyrd::Texture** operator<<(Wyrd::Texture** data, const jsonxx::String val)
	{
		(*data) = Resources::Get().Textures[UID(val.c_str())].get();
		return data;
	}

	Wyrd::UID& operator<<(Wyrd::UID& data, const jsonxx::String val){
		data = UID(val.c_str());
		return data;
	}

	Wyrd::Rect& operator<<(Wyrd::Rect& data, const jsonxx::Object& val) {
		data._position.x = (float)val.get<jsonxx::Number>("x");
		data._position.y = (float)val.get<jsonxx::Number>("y");
		data._size.x = (float)val.get<jsonxx::Number>("w");
		data._size.y = (float)val.get<jsonxx::Number>("h");
		return data;
	}
}