#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <jsonxx.h>
#include <glm/glm.hpp>

using namespace jsonxx;

namespace Wyrd
{
	class WYRD_LIBRARY_API ComponentSerialiserFactory
	{
	public:
		static Object Serialise(const std::string& name, const char* data)
		{
			Object obj;

			if (strcmp(name.c_str(), "MetaData") == 0)
			{
				Serialise(obj, (MetaDataComponent*)data);
			}
			else if (strcmp(name.c_str(), "Transform2D") == 0)
			{
				Serialise(obj, (Transform2DComponent*)data);
			}
			else if (strcmp(name.c_str(), "Sprite") == 0)
			{
				Serialise(obj, (SpriteComponent*)data);
			}
			else if (strcmp(name.c_str(), "Script") == 0)
			{
				Serialise(obj, (ScriptComponent*)data);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Serialise(obj, (CameraComponent*)data);
			}

			return obj;
		}

		static void Deserialise(const std::string& name, Object& obj, char* buffer)
		{
			if (strcmp(name.c_str(), "MetaData") == 0)
			{
				Deserialise(obj, (MetaDataComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Transform2D") == 0)
			{
				Deserialise(obj, (Transform2DComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Sprite") == 0)
			{
				Deserialise(obj, (SpriteComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Script") == 0)
			{
				Deserialise(obj, (ScriptComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Deserialise(obj, (CameraComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Deserialise(obj, (EditorComponent*)buffer);
			}
		}

		static void Serialise(Object& obj, const Wyrd::MetaDataComponent* data);
		static void Serialise(Object& obj, const Wyrd::Transform2DComponent* data);
		static void Serialise(Object& obj, const Wyrd::SpriteComponent* data);
		static void Serialise(Object& obj, const Wyrd::ScriptComponent* data);
		static void Serialise(Object& obj, const Wyrd::CameraComponent* data);
		static void Serialise(Object& obj, const Wyrd::EditorComponent* data);

		static void Deserialise(Object& obj, Wyrd::MetaDataComponent* data);
		static void Deserialise(Object& obj, Wyrd::Transform2DComponent* data);
		static void Deserialise(Object& obj, Wyrd::SpriteComponent* data);
		static void Deserialise(Object& obj, Wyrd::ScriptComponent* data);
		static void Deserialise(Object& obj, Wyrd::CameraComponent* data);
		static void Deserialise(Object& obj, Wyrd::EditorComponent* data);
	};
};