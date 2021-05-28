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
				Serialise(obj, (ECSTransform2DComponent*)data);
			}
			else if (strcmp(name.c_str(), "Sprite") == 0)
			{
				Serialise(obj, (ECSSpriteComponent*)data);
			}
			else if (strcmp(name.c_str(), "Script") == 0)
			{
				Serialise(obj, (ECSScriptComponent*)data);
			}
			else if (strcmp(name.c_str(), "ScriptInternal") == 0)
			{
				Serialise(obj, (ECSScriptInternalComponent*)data);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Serialise(obj, (ECSCameraComponent*)data);
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
				Deserialise(obj, (ECSTransform2DComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Sprite") == 0)
			{
				Deserialise(obj, (ECSSpriteComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Script") == 0)
			{
				Deserialise(obj, (ECSScriptComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "ScriptInternal") == 0)
			{
				Deserialise(obj, (ECSScriptInternalComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Deserialise(obj, (ECSCameraComponent*)buffer);
			}
			else if (strcmp(name.c_str(), "Camera") == 0)
			{
				Deserialise(obj, (ECSEditorComponent*)buffer);
			}
		}

		static void Serialise(Object& obj, const Wyrd::MetaDataComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSTransform2DComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSSpriteComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSScriptComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSScriptInternalComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSCameraComponent* data);
		static void Serialise(Object& obj, const Wyrd::ECSEditorComponent* data);

		static void Deserialise(Object& obj, Wyrd::MetaDataComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSTransform2DComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSSpriteComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSScriptComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSScriptInternalComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSCameraComponent* data);
		static void Deserialise(Object& obj, Wyrd::ECSEditorComponent* data);
	};
};