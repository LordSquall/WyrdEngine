#pragma once
#include "osrpch.h"

#include "Transform3DComponent.h"

namespace Osiris
{
	Transform3DComponent::Transform3DComponent(std::shared_ptr<GameObject> gameObject) : IBaseComponent(gameObject, SceneComponentType::Transform3D)
	{
		Initialised = true;
	}

	Transform3DComponent::Transform3DComponent(const Transform3DComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::Transform3D)
	{
		position = obj.position;
		rotation = obj.rotation; 
		scale = obj.scale;

		Initialised = true;
	}

	Transform3DComponent::~Transform3DComponent()
	{

	}
}