#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Wyrd
{
	class WYRD_LIBRARY_API ComponentSerialiserFactory
	{
	public:
		static jsonxx::Object Serialise(const std::string& name, const char* data)
		{
			jsonxx::Object obj;

			if (strcmp(name.c_str(), "Unknown") == 0)
			{
				//TODO: Throw Assert
			}
            else if (strcmp(name.c_str(), "MetaData") == 0)
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
            else if (strcmp(name.c_str(), "Relationship") == 0)
            {
                Serialise(obj, (RelationshipComponent*)data);
            }

	
			return obj;
		}

		static void Deserialise(const std::string& name, jsonxx::Object& obj, char* buffer)
		{
			if (strcmp(name.c_str(), "Uknown") == 0)
			{
				//TODO: Throw Assert
			}
            else if (strcmp(name.c_str(), "MetaData") == 0)
            {
                MetaDataComponent* comp = new (buffer) MetaDataComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "Transform2D") == 0)
            {
                Transform2DComponent* comp = new (buffer) Transform2DComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "Sprite") == 0)
            {
                SpriteComponent* comp = new (buffer) SpriteComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "Script") == 0)
            {
                ScriptComponent* comp = new (buffer) ScriptComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "Relationship") == 0)
            {
                RelationshipComponent* comp = new (buffer) RelationshipComponent();
                Deserialise(obj, comp);
            }


		}


        static void Serialise(jsonxx::Object& obj, Wyrd::MetaDataComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::MetaDataComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::Transform2DComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::Transform2DComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::SpriteComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::SpriteComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::ScriptComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::ScriptComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::RelationshipComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::RelationshipComponent* data);

	};
};