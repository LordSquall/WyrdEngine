#pragma once
#include "wyrdpch.h"

/* local includes  */
#include "core/Application.h"
#include "core/ecs/Components.h"
#include "core/behaviour/ScriptedClass.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"

using namespace jsonxx;

namespace Wyrd
{
	void ScriptComponentPropSerialise(jsonxx::Object& prop, const Wyrd::ScriptComponent* data, std::pair<std::string, std::shared_ptr<ScriptProperty>> classProp, int index)
	{
		if (strcmp(classProp.second->GetTypeName().c_str(), "Int32") == 0)
		{
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *(int*)data->properties[index];
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Single") == 0)
		{
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *(float*)data->properties[index];
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Boolean") == 0)
		{
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *(bool*)data->properties[index];
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "String") == 0)
		{
			std::string valStr = std::string(data->properties[index]);
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << valStr;
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Color") == 0)
		{
			Color* val = (Color*)data->properties[index];
			jsonxx::Array valArr;
			valArr << val->r;
			valArr << val->g;
			valArr << val->b;
			valArr << val->a;

			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << valArr;
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Entity") == 0)
		{
			Entity* val = (Entity*)data->properties[index];
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *val;
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Vector2") == 0)
		{
			Vector2* val = (Vector2*)data->properties[index];
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *val;
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Vector3") == 0)
		{
			Vector3* val = (Vector3*)data->properties[index];
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << *val;
		}
		else if (strcmp(classProp.second->GetTypeName().c_str(), "Texture") == 0)
		{
			std::string valStr = std::string(data->properties[index]);
			prop << "name" << classProp.first;
			prop << "type" << classProp.second->GetTypeName();
			prop << "data" << valStr;
		}
	}

	void ScriptComponentPropDeserialise(jsonxx::Object& prop, Wyrd::ScriptComponent* data, int index)
	{
		if (prop.has<jsonxx::String>("type") && prop.has<jsonxx::String>("name"))
		{
			std::string propType = prop.get<jsonxx::String>("type");
			std::string propName = prop.get<jsonxx::String>("name");

			data->propertyMap.insert({ propName, (void*)data->properties[index] });

			if (strcmp(propType.c_str(), "Int32") == 0)
			{
				int32_t val = (int32_t)prop.get<jsonxx::Number>("data");
				memcpy((void*)data->properties[index], &val, sizeof(int32_t));
			}
			else if (strcmp(propType.c_str(), "Single") == 0)
			{
				float val = (float)prop.get<jsonxx::Number>("data");
				memcpy((void*)data->properties[index], &val, sizeof(float));
			}
			else if (strcmp(propType.c_str(), "Boolean") == 0)
			{
				bool val = (bool)prop.get<jsonxx::Boolean>("data");
				memcpy((void*)data->properties[index], &val, sizeof(bool));
			}
			else if (strcmp(propType.c_str(), "String") == 0)
			{
				std::string valStr = prop.get<jsonxx::String>("data");
				std::strncpy(data->properties[index], valStr.c_str(), valStr.length());
				data->properties[index][valStr.length()] = '\0';
			}
			else if (strcmp(propType.c_str(), "Color") == 0)
			{
				jsonxx::Array valStr = prop.get<jsonxx::Array>("data");
				Color val = {
					valStr.get<jsonxx::Number>(0),
					valStr.get<jsonxx::Number>(1),
					valStr.get<jsonxx::Number>(2),
					valStr.get<jsonxx::Number>(3),
				};

				memcpy((void*)data->properties[index], &val, sizeof(Color));
			}
			else if (strcmp(propType.c_str(), "Entity") == 0)
			{
				Entity val = prop.get<jsonxx::Number>("data");
				memcpy((void*)data->properties[index], &val, sizeof(Entity));
			}
			else if (strcmp(propType.c_str(), "Vector2") == 0)
			{
				Vector2 val;
				val << prop.get<jsonxx::Array>("data");
				memcpy((void*)data->properties[index], &val, sizeof(Vector2));
			}
			else if (strcmp(propType.c_str(), "Vector3") == 0)
			{
				Vector3 val;
				val << prop.get<jsonxx::Array>("data");
				memcpy((void*)data->properties[index], &val, sizeof(Vector3));
			}
			else if (strcmp(propType.c_str(), "Texture") == 0)
			{
				std::string valStr = prop.get<jsonxx::String>("data");
				strcpy(data->properties[index], valStr.c_str());
			}
		}
	}

	/* serialise */
	void ComponentSerialiserFactory::Serialise(Object& obj, Wyrd::MetaDataComponent* data)
	{
		obj << "name" << data->name;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, Wyrd::Transform2DComponent* data)
	{
		obj << "position" << data->position;
		obj << "rotation" << data->rotation;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, Wyrd::SpriteComponent* data)
	{
		obj << "position" << data->position;
		obj << "size" << data->size;
		obj << "color" << data->color;
		obj << "texture" << data->texture;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, Wyrd::ScriptComponent* data)
	{
		obj << "script" << data->script;

		
		/* retrieve script class */
		UID uid = data->script;
		auto scriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(uid);
		
		jsonxx::Array properties;
		if (scriptClass != nullptr)
		{
			int i = 0;
			for (auto& classProp : scriptClass->Properties)
			{
				jsonxx::Object prop;
				//classProp.second->ToJson(prop, data, i);
				ScriptComponentPropSerialise(prop, data, classProp, i);
				properties << prop;
				i++;
			}
		}
		obj << "properties" << properties;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, Wyrd::CameraComponent* data)
	{
		obj << "size" << data->size;
	}

	/* deserialise */
	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::MetaDataComponent* data)
	{
		std::string name = obj.get<String>("name");
		strcpy(data->name, name.c_str());
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::Transform2DComponent* data)
	{
		data->position << obj.get<Array>("position");
		data->rotation = (float)obj.get<Number>("rotation");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::SpriteComponent* data)
	{
		data->position << obj.get<Array>("position");
		data->size << obj.get<Array>("size");
		data->color << obj.get<Array>("color");
		data->texture << obj.get<Array>("texture");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ScriptComponent* data)
	{
		data->script << obj.get<Array>("script");

		jsonxx::Array props = obj.get<Array>("properties");

		for (int i = 0; i < props.size(); i++)
		{
			jsonxx::Object propObj = props.get<jsonxx::Object>(i);
			
			ScriptComponentPropDeserialise(propObj, data, i);
		}
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::CameraComponent* data)
	{
		data->size = obj.get<Number>("size");
	}
}