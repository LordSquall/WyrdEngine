#pragma once

/* local includes */
#include "osrpch.h"
#include "GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/Transform3DComponent.h"
#include "core/scene/components/SpriteComponent.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/scene/components/PhysicsComponent.h"


namespace Osiris
{
	uint32_t GameObject::_nextUid = 1u;

	GameObject::GameObject() : name("Untitled"), _uid(_nextUid)
	{
		_nextUid++;
	}

	GameObject::GameObject(std::string name) : name(name), _uid(_nextUid)
	{
		_nextUid++;
	}

	GameObject::GameObject(const GameObject& obj) : _uid(_nextUid)
	{
		name = obj.name;
		components = obj.components;
		inputArea = obj.inputArea;

		_nextUid++;
	}

	GameObject::~GameObject() { }
}