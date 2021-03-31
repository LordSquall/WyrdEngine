#pragma once
#include "osrpch.h"
#include "core/Log.h"
#include "CameraComponent.h"
#include "core/scene/GameObject.h"
#include "serial/TypeSerialisers.h"

namespace Osiris
{
	CameraComponent::CameraComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::CameraComponent), _Position(0.0f, 0.0f)
	{
	}

	jsonxx::Object CameraComponent::ToJson()
	{
		jsonxx::Object object;

		object << "componentType" << (int)SceneComponentType::CameraComponent;

		/* position */
		object << "position" << _Position;

		return object;
	}

	bool CameraComponent::FromJson(jsonxx::Object& object)
	{
		/* position */
		if (object.has<jsonxx::Array>("position"))
			_Position << object.get<jsonxx::Array>("position");

		return true;
	}
}