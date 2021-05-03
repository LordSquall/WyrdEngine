#pragma once
#include "wyrdpch.h"
#include "core/Log.h"
#include "CameraComponent.h"
#include "core/scene/GameObject.h"
#include "serial/TypeSerialisers.h"

#include <jsonxx.h>

namespace Wyrd
{
	CameraComponent::CameraComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::CameraComponent), _Size(0.0f), _CameraUID(UIDUtils::Create())
	{
		_Camera = std::make_unique<OrthographicCamera>();
	}

	bool CameraComponent::ToJson(jsonxx::Object& object)
	{
		object << "componentType" << (int)SceneComponentType::CameraComponent;

		/* size */
		object << "size" << _Size;

		/* camera uid */
		object << "camerauid" << _CameraUID.str();

		return true;
	}

	bool CameraComponent::FromJson(jsonxx::Object& object)
	{
		/* size */
		if (object.has<jsonxx::Number>("size"))
			_Size = (float)object.get<jsonxx::Number>("size");

		/* camera uid */
		if (object.has<jsonxx::String>("camerauid"))
			_CameraUID = UID(object.get<jsonxx::String>("camerauid", "MISSING"));

		return true;
	}
}