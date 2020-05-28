#pragma once
#include "osrpch.h"

#include "GameObject.h"

namespace Osiris::Editor
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
		transform2d = obj.transform2d;
		spriteRender = obj.spriteRender;
		script = obj.script;
		inputArea = obj.inputArea;

		_nextUid++;
	}

	GameObject::~GameObject() { }
}