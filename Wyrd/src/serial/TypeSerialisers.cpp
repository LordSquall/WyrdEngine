#include "wyrdpch.h"

/* local includes  */
#include "core/UID.h"
#include "serial/TypeSerialisers.h"

namespace glm
{
	// jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec2& vector) {
		// jsonxx::Array arr;
		// arr << vector.x << vector.y;
		// obj << arr;
		// return obj;
	// }
// 
	// glm::vec2& operator<<(glm::vec2& vector, const jsonxx::Array& arr) {
		// vector.x = (float)arr.get<jsonxx::Number>(0);
		// vector.y = (float)arr.get<jsonxx::Number>(1);
		// return vector;
	// }
// 
	// jsonxx::Object& operator<<(jsonxx::Object& obj, const glm::vec3& vector) {
		// jsonxx::Array arr;
		// arr << vector.x << vector.y << vector.z;
		// obj << arr;
		// return obj;
	// }
// 
	// glm::vec3& operator<<(glm::vec3& vector, const jsonxx::Array& arr) {
		// vector.x = (float)arr.get<jsonxx::Number>(0);
		// vector.y = (float)arr.get<jsonxx::Number>(1);
		// vector.z = (float)arr.get<jsonxx::Number>(2);
		// return vector;
	// }
}

namespace Wyrd
{
	// jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Color& color) {
	// 	jsonxx::Array arr;
	// 	arr << color.r << color.g << color.b << color.a;
	// 	obj << arr;
	// 	return obj;
	// }

	// Wyrd::Color& operator<<(Wyrd::Color& color, const jsonxx::Array& arr) {
	// 	color.r = (float)arr.get<jsonxx::Number>(0);
	// 	color.g = (float)arr.get<jsonxx::Number>(1);
	// 	color.b = (float)arr.get<jsonxx::Number>(2);
	// 	color.a = (float)arr.get<jsonxx::Number>(3);
	// 	return color;
	// }

	// jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Vector2& vector) {
	// 	jsonxx::Array arr;
	// 	arr << vector.x << vector.y;
	// 	obj << arr;
	// 	return obj;
	// }

	// Wyrd::Vector2& operator<<(Wyrd::Vector2& vector, const jsonxx::Array& arr) {
	// 	vector.x = (float)arr.get<jsonxx::Number>(0);
	// 	vector.y = (float)arr.get<jsonxx::Number>(1);
	// 	return vector;
	// }

	// jsonxx::Object& operator<<(jsonxx::Object& obj, const Wyrd::Vector3& vector) {
	// 	jsonxx::Array arr;
	// 	arr << vector.x << vector.y << vector.z;
	// 	obj << arr;
	// 	return obj;
	// }

	// Wyrd::Vector3& operator<<(Wyrd::Vector3& vector, const jsonxx::Array& arr) {
	// 	vector.x = (float)arr.get<jsonxx::Number>(0);
	// 	vector.y = (float)arr.get<jsonxx::Number>(1);
	// 	vector.z = (float)arr.get<jsonxx::Number>(2);
	// 	return vector;
	// }

	// jsonxx::Object& operator<<(jsonxx::Object& obj, const UID& uid) {
	// 	jsonxx::Array arr;
	// 	arr << uid.str();
	// 	obj << arr;
	// 	return obj;
	// }

	// UID& operator<<(UID& uid, const jsonxx::Array& arr) {
	// 	std::string uidStr = arr.get<jsonxx::String>(0);
	// 	uid = UID(uidStr);
	// 	return uid;
	// }
}