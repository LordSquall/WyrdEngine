#pragma once

/* local includes  */
#include "core/Structures.h"
#include "properties/BaseProp.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Wyrd
{
	// Int32
	//void BaseProp<int32_t>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "Int32";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<int32_t>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value = json.get<jsonxx::Number>("value", 0);
	//}
	//
	//
	//// UInt32
	//void BaseProp<uint32_t>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "UInt32";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<uint32_t>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value = json.get<jsonxx::Number>("value", 0);
	//}
	//
	//// Int64
	//void BaseProp<int64_t>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "Int64";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<int64_t>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value = json.get<jsonxx::Number>("value", 0);
	//}
	//
	//// UInt64
	//void BaseProp<uint64_t>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "UInt64";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<uint64_t>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value = json.get<jsonxx::Number>("value", 0);
	//}
	//
	//// String
	//void BaseProp<std::string>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "String";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<std::string>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value = json.get<jsonxx::String>("value", "");
	//}
	//
	//// Vector 2
	//void BaseProp<Vector2>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "Vec2";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<Vector2>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value << json.get<jsonxx::Object>("value");
	//}
	//
	//// Vector 3
	//void BaseProp<Vector3>::Serialise(jsonxx::Object& json)
	//{
	//	jsonxx::Object obj;
	//	obj << "type" << "Vec3";
	//	obj << "value" << _Value;
	//	json << _Name << obj;
	//}
	//
	//void BaseProp<Vector3>::Deserialise(const jsonxx::Object& json)
	//{
	//	_Value << json.get<jsonxx::Object>("value");
	//}
}