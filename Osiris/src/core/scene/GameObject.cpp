#pragma once

/* local includes */
#include "osrpch.h"
#include "GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/Transform3DComponent.h"
#include "core/scene/components/SpriteComponent.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/behaviour/ScriptedClass.h"


namespace Osiris
{
	GameObject::GameObject() : name("Untitled")
	{
	}

	GameObject::GameObject(std::string name) : name(name)
	{
	}

	GameObject::GameObject(const GameObject& obj)
	{
		name = obj.name;
		components = obj.components;
		inputArea = obj.inputArea;

		uid = obj.uid;
	}

	GameObject::~GameObject() { }

	void GameObject::AddChild(std::shared_ptr<GameObject> gameObject)
	{
		children.push_back(gameObject);
	}

	void GameObject::RemoveChild(UID uid)
	{
		children.erase(std::remove_if(children.begin(), children.end(),
			[&](const std::shared_ptr<GameObject>& child)
			{ return child->uid == uid; }),
			children.end());
	}

	void GameObject::DuplicateChild(UID uid)
	{
		// TODO
	}

	void GameObject::SwapChild(int a, int b)
	{
		std::shared_ptr<GameObject> tmp = children[a];
		children[a] = children[b];
		children[b] = tmp;
	}

	std::shared_ptr<ScriptComponent> GameObject::FindScriptComponent(const std::string& name)
	{
		for (auto& component : components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				std::shared_ptr<ScriptComponent> sc = std::dynamic_pointer_cast<ScriptComponent>(component);

				if (sc->GetClass()->GetName() == name)
				{
					return sc;
				}
			}
		}

		return nullptr;
	}


	std::shared_ptr<GameObject> GameObject::FindChild(const UID uid)
	{
		for (auto& child : children)
		{
			if (child->uid == uid)
			{
				return child;
			}

			auto found = child->FindChild(uid);
			if (found != nullptr)
				return found;
		}

		return nullptr;
	}
}