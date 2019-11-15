#pragma once
#include "osrpch.h"

#include "GameObject.h"

namespace Osiris::Editor
{
	GameObject::GameObject() : name("Untitled") { }

	GameObject::GameObject(std::string name) : name(name) { }

	GameObject::GameObject(const GameObject& obj)
	{
		name = obj.name;
		transform2d = obj.transform2d;
		transform3d = obj.transform3d;
		spriteRender = obj.spriteRender;
	}

	GameObject::~GameObject() { }
}