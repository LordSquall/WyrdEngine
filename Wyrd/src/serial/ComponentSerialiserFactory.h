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
            else if (strcmp(name.c_str(), "Transform3D") == 0)
            {
                Serialise(obj, (Transform3DComponent*)data);
            }
            else if (strcmp(name.c_str(), "MeshRenderer") == 0)
            {
                Serialise(obj, (MeshRendererComponent*)data);
            }
            else if (strcmp(name.c_str(), "Material") == 0)
            {
                Serialise(obj, (MaterialComponent*)data);
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
            else if (strcmp(name.c_str(), "Camera") == 0)
            {
                Serialise(obj, (CameraComponent*)data);
            }

	
			return obj;
		}

		static void Serialise(std::ofstream& stream, const std::string& name, const char* data)
		{
			if (strcmp(name.c_str(), "Unknown") == 0)
			{
				//TODO: Throw Assert
			}
            else if (strcmp(name.c_str(), "MetaData") == 0)
            {
                Serialise(stream, (MetaDataComponent*)data);
            }
            else if (strcmp(name.c_str(), "Transform2D") == 0)
            {
                Serialise(stream, (Transform2DComponent*)data);
            }
            else if (strcmp(name.c_str(), "Transform3D") == 0)
            {
                Serialise(stream, (Transform3DComponent*)data);
            }
            else if (strcmp(name.c_str(), "MeshRenderer") == 0)
            {
                Serialise(stream, (MeshRendererComponent*)data);
            }
            else if (strcmp(name.c_str(), "Material") == 0)
            {
                Serialise(stream, (MaterialComponent*)data);
            }
            else if (strcmp(name.c_str(), "Sprite") == 0)
            {
                Serialise(stream, (SpriteComponent*)data);
            }
            else if (strcmp(name.c_str(), "Script") == 0)
            {
                Serialise(stream, (ScriptComponent*)data);
            }
            else if (strcmp(name.c_str(), "Relationship") == 0)
            {
                Serialise(stream, (RelationshipComponent*)data);
            }
            else if (strcmp(name.c_str(), "Camera") == 0)
            {
                Serialise(stream, (CameraComponent*)data);
            }

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
            else if (strcmp(name.c_str(), "Transform3D") == 0)
            {
                Transform3DComponent* comp = new (buffer) Transform3DComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "MeshRenderer") == 0)
            {
                MeshRendererComponent* comp = new (buffer) MeshRendererComponent();
                Deserialise(obj, comp);
            }
            else if (strcmp(name.c_str(), "Material") == 0)
            {
                MaterialComponent* comp = new (buffer) MaterialComponent();
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
            else if (strcmp(name.c_str(), "Camera") == 0)
            {
                CameraComponent* comp = new (buffer) CameraComponent();
                Deserialise(obj, comp);
            }


		}

		static void Deserialise(const std::string& name, std::ifstream& stream, char* buffer)
		{
			if (strcmp(name.c_str(), "Uknown") == 0)
			{
				//TODO: Throw Assert
			}
            else if (strcmp(name.c_str(), "MetaData") == 0)
            {
                MetaDataComponent* comp = new (buffer) MetaDataComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Transform2D") == 0)
            {
                Transform2DComponent* comp = new (buffer) Transform2DComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Transform3D") == 0)
            {
                Transform3DComponent* comp = new (buffer) Transform3DComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "MeshRenderer") == 0)
            {
                MeshRendererComponent* comp = new (buffer) MeshRendererComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Material") == 0)
            {
                MaterialComponent* comp = new (buffer) MaterialComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Sprite") == 0)
            {
                SpriteComponent* comp = new (buffer) SpriteComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Script") == 0)
            {
                ScriptComponent* comp = new (buffer) ScriptComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Relationship") == 0)
            {
                RelationshipComponent* comp = new (buffer) RelationshipComponent();
                Deserialise(stream, comp);
            }
            else if (strcmp(name.c_str(), "Camera") == 0)
            {
                CameraComponent* comp = new (buffer) CameraComponent();
                Deserialise(stream, comp);
            }


		}


        static void Serialise(jsonxx::Object& obj, Wyrd::MetaDataComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::MetaDataComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::MetaDataComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::MetaDataComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::Transform2DComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::Transform2DComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::Transform2DComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::Transform2DComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::Transform3DComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::Transform3DComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::Transform3DComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::Transform3DComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::MeshRendererComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::MeshRendererComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::MeshRendererComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::MeshRendererComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::MaterialComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::MaterialComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::MaterialComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::MaterialComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::SpriteComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::SpriteComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::SpriteComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::SpriteComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::ScriptComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::ScriptComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::ScriptComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::ScriptComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::RelationshipComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::RelationshipComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::RelationshipComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::RelationshipComponent* data);

        static void Serialise(jsonxx::Object& obj, Wyrd::CameraComponent* data);
        static void Serialise(std::ofstream& stream, Wyrd::CameraComponent* data);
        static void Deserialise(jsonxx::Object& obj, Wyrd::CameraComponent* data);
        static void Deserialise(std::ifstream& stream, Wyrd::CameraComponent* data);

	};
};