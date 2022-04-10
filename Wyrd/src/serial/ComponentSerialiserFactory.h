#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>

using namespace jsonxx;

namespace Wyrd
{
	class WYRD_LIBRARY_API ComponentSerialiserFactory
	{
	public:
		//static Object Serialise(const std::string& name, const char* data)
		//{
		//	return obj;
		//}

		//static void Deserialise(const std::string& name, Object& obj, char* buffer)
		//{
		//	
		//}

		//static void Serialise(Object& obj, Wyrd::MetaDataComponent* data);
		//static void Serialise(Object& obj, Wyrd::Transform2DComponent* data);
		//static void Serialise(Object& obj, Wyrd::SpriteComponent* data);
		//static void Serialise(Object& obj, Wyrd::ScriptComponent* data);
		//static void Serialise(Object& obj, Wyrd::CameraComponent* data);
		//static void Serialise(Object& obj, Wyrd::TextComponent* data);
		//static void Serialise(Object& obj, Wyrd::RelationshipComponent* data);
		//
		//static void Deserialise(Object& obj, Wyrd::MetaDataComponent* data);
		//static void Deserialise(Object& obj, Wyrd::Transform2DComponent* data);
		//static void Deserialise(Object& obj, Wyrd::SpriteComponent* data);
		//static void Deserialise(Object& obj, Wyrd::ScriptComponent* data);
		//static void Deserialise(Object& obj, Wyrd::CameraComponent* data);
		//static void Deserialise(Object& obj, Wyrd::TextComponent* data);
		//static void Deserialise(Object& obj, Wyrd::RelationshipComponent* data);
	};
};