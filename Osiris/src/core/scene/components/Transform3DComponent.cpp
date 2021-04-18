#pragma once
#include "osrpch.h"

#include "Transform3DComponent.h"

namespace Osiris
{
	Transform3DComponent::Transform3DComponent(GameObject* gameObject) : TransformComponent(gameObject, SceneComponentType::Transform3D)
	{
		Initialised = true;
	}

	Transform3DComponent::~Transform3DComponent()
	{

	}

	bool Transform3DComponent::ToJson(jsonxx::Object& object)
	{
		return true;
	}

	bool Transform3DComponent::FromJson(jsonxx::Object& object)
	{
		return true;
	}
}