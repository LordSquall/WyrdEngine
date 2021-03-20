#pragma once

/* local includes */
#include "osrpch.h"
#include "GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/Transform3DComponent.h"
#include "core/scene/components/SpriteComponent.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"

namespace Osiris
{
	GameObject::GameObject() : name("Untitled")
	{
	}

	GameObject::GameObject(std::string name, bool generateUID) : name(name)
	{
		if (generateUID == true)
			uid = UIDUtils::Create();
	}

	GameObject::~GameObject() { }

	GameObject* GameObject::AddChild(std::unique_ptr<GameObject> gameObject)
	{
		gameObject->layer = layer;
	
		_GameObjects.push_back(std::move(gameObject));

		return &*_GameObjects.back();
	}

	void GameObject::RemoveChild(UID uid)
	{
		/* retrieve the gameobject with matching id */
		auto gameObject = std::find_if(children.begin(), children.end(), [&uid](const std::shared_ptr<GameObject>& c) { return c->uid == uid; });

		if (gameObject != children.end())
		{
			/* remove all the children if any */
			for (auto& child : (*gameObject)->children)
			{
				child->RemoveChildren();
			}

			/* process each component to remove any additional links */
			for (auto& component : (*gameObject)->components)
			{
				component->Remove();
			}

			children.erase(gameObject);
		}
	}

	void GameObject::RemoveChildren()
	{
		/* remove all the children if any */
		for (auto& child : children)
		{
			child->RemoveChildren();
		}

		/* process each component to remove any additional links */
		for (auto& component : components)
		{
			component->Remove();
		}
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

	void GameObject::AddComponent(std::unique_ptr<IBaseComponent> component)
	{
		component->Setup();

		components.push_back(std::move(component));
	}

	ScriptComponent* GameObject::FindScriptComponent(const std::string& name)
	{
		for (auto& component : components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* sc = dynamic_cast<ScriptComponent*>(component.get());

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


	void GameObject::AssignScripts(Behaviour* behaviour)
	{
		for (auto& component : components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* sc = (ScriptComponent*)&*component;

				std::string oldName = sc->GetClass()->GetName();
				std::shared_ptr<PropertyList_t> oldProperties = sc->Properties;

				sc->SetClass(behaviour->GetCustomClass(oldName));
				
				for (auto& prop : *oldProperties)
				{
					auto oldProp = oldProperties->find(prop.first);
					auto newProp = sc->Properties->find(prop.first);

					if (oldProp != oldProperties->end() && newProp != sc->Properties->end())
					{
						newProp->second->CopyValue(oldProp->second);
					}
				}
			}
		}

		for (auto& go : children)
		{
			go->AssignScripts(behaviour);
		}
	}

	jsonxx::Object GameObject::ToJson()
	{
		jsonxx::Object object;

		/* name */
		object << "name" << name;

		/* uid */
		object << "uid" << uid.str();

		/* transform component */
		object << "transform2D" << transform->ToJson();

		return object;
	}

	bool GameObject::FromJson(jsonxx::Object& object)
	{
		/* name */
		name = object.get<jsonxx::String>("name", "MISSING");

		/* uid */
		uid = UID(object.get<jsonxx::String>("uid", "MISSING"));

		/* transform component */
		if (object.has<jsonxx::Object>("transform2D") == true)
		{
			std::unique_ptr<Transform2DComponent> transform2DComponent = std::make_unique<Transform2DComponent>(this);
			transform2DComponent->FromJson(object.get<jsonxx::Object>("transform2D"));
			transform = std::move(transform2DComponent);
		}

		return true;
	}
}