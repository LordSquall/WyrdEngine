#pragma once
#include "wyrdpch.h"

/* local includes  */
#include "core/Log.h"
#include "core/Application.h"
#include "core/ecs/Components.h"
#include "core/behaviour/ScriptedClass.h"
#include "serial/TypeSerialisers.h"
#include "ComponentSerialiserFactory.h"

void ComponentSerialiserFactory::Deserialise(const std::string& name, std::ifstream& stream, char* buffer)
{
	if (strcmp(name.c_str(), "MetaData") == 0)
	{
		MetaDataComponent* comp = new (buffer) MetaDataComponent();
		Deserialise(stream, comp);
	}
	else if (strcmp(name.c_str(), "Transform2D") == 0)
	{
		Transform2DComponent* comp = new (buffer) Transform2DComponent();
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
	else if (strcmp(name.c_str(), "Camera") == 0)
	{
		CameraComponent* comp = new (buffer) CameraComponent();
		Deserialise(stream, comp);
	}
	else if (strcmp(name.c_str(), "Text") == 0)
	{
		TextComponent* comp = new (buffer) TextComponent();
		Deserialise(stream, comp);
	}
}

/* deserialise */
void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::MetaDataComponent* data)
{
	stream.read(data->name, sizeof(char) * METADATA_COMP_NAME_LENGTH);
}

void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::Transform2DComponent* data)
{
	stream.read((char*)&data->position, sizeof(glm::vec2));
	stream.read((char*)&data->rotation, sizeof(float));
}

void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::SpriteComponent* data)
{
	stream.read((char*)&data->position, sizeof(glm::vec2));
	stream.read((char*)&data->size, sizeof(glm::vec2));
	stream.read((char*)&data->tiling, sizeof(glm::vec2));
	stream.read((char*)&data->color, sizeof(Wyrd::Color));
	stream.read((char*)&data->texture, sizeof(UID));
}

void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::ScriptComponent* data)
{
	stream.read((char*)&data->script, sizeof(UID));
	stream.read((char*)&data->instanceID, sizeof(int32_t));
	stream.read((char*)&data->propertyCount, sizeof(int32_t));
	stream.read((char*)&data->properties, sizeof(char) * (SCRIPT_COMP_PROP_MAX * SCRIPT_COMP_PROP_DATA_LENGTH));

	stream.ignore(sizeof(std::map<std::string, void*>));

	auto scriptedClass = Application::Get().GetBehaviour().GetCustomClassByUID(data->script);

	if (scriptedClass != nullptr)
	{
		int i = 0;
		for (auto& o : scriptedClass->Properties)
		{
			data->propertyMap.insert({ o.second->GetName(),  data->properties + (i * SCRIPT_COMP_PROP_DATA_LENGTH) });
			i++;
		}
	}
	else
	{
		WYRD_CORE_ERROR("Unable to find matching class!");

	}
}

void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::CameraComponent* data)
{
	stream.read((char*)&data->viewport, sizeof(Rect));
	stream.read((char*)&data->aspectRatio, sizeof(float));
	stream.read((char*)&data->size, sizeof(float));
}

void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::TextComponent* data)
{
	stream.read(data->content, sizeof(char) * TEXT_COMP_MAX_LENGTH);
	stream.read((char*)&data->color, sizeof(Wyrd::Color));
	stream.read(data->font, sizeof(char) * TEXT_COMP_FONT_NAME_MAX_LENGTH);
}