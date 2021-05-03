#pragma once
#include "wyrdpch.h"
#include "core/Log.h"
#include "TransformComponent.h"
#include "core/scene/GameObject.h"


namespace Wyrd
{
	TransformComponent::TransformComponent(GameObject* gameObject, SceneComponentType componentType) : IBaseComponent(gameObject, SceneComponentType::Transform2D)
	{

	}

	TransformComponent::TransformComponent(const TransformComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::Transform2D)
	{

	}
}