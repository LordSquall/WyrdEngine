#pragma once
#include "wyrdpch.h"

/* local includes  */
#include "core/ecs/Components.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"

using namespace jsonxx;

namespace Wyrd
{
	/* serialise */
	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::MetaDataComponent* data)
	{
		obj << "name" << data->name;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSTransform2DComponent* data)
	{
		obj << "position" << data->position;
		obj << "rotation" << data->rotation;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSSpriteComponent* data)
	{
		obj << "position" << data->position;
		obj << "size" << data->size;
		obj << "color" << data->color;
		obj << "texture" << data->texture;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSScriptComponent* data)
	{
		obj << "script" << data->script;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSScriptInternalComponent* data)
	{
		obj << "instanceID" << data->instanceID;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSCameraComponent* data)
	{
		obj << "size" << data->size;
	}

	void ComponentSerialiserFactory::Serialise(Object& obj, const Wyrd::ECSEditorComponent* data)
	{
		obj << "inputArea" << data->inputArea;
	}

	/* deserialise */
	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::MetaDataComponent* data)
	{
		std::string name = obj.get<String>("name");
		strcpy(data->name, name.c_str());
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSTransform2DComponent* data)
	{
		data->position << obj.get<Array>("position");
		data->rotation = (float)obj.get<Number>("rotation");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSSpriteComponent* data)
	{
		data->position << obj.get<Array>("position");
		data->size << obj.get<Array>("size");
		data->color << obj.get<Array>("color");
		data->texture << obj.get<Array>("texture");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSScriptComponent* data)
	{
		data->script << obj.get<Array>("script");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSScriptInternalComponent* data)
	{
		data->instanceID = obj.get<Number>("instanceID");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSCameraComponent* data)
	{
		data->size = obj.get<Number>("size");
	}

	void ComponentSerialiserFactory::Deserialise(Object& obj, Wyrd::ECSEditorComponent* data)
	{

	}
}