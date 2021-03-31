#pragma once

/* local includes */
#include "osrpch.h"
#include "GameObject.h"
#include "core/scene/components/SceneComponentFactory.h"
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

	void GameObject::RemoveChild(UID uid)
	{
		/* retrieve the gameobject with matching id */
		auto gameObject = std::find_if(_GameObjects.begin(), _GameObjects.end(), [&uid](const std::unique_ptr<GameObject>& c) { return c->uid == uid; });

		if (gameObject != _GameObjects.end())
		{
			/* remove all the children if any */
			for (auto& child : (*gameObject)->GetGameObjects())
			{
				child->RemoveChildren();
			}

			/* process each component to remove any additional links */
			for (auto& component : (*gameObject)->components)
			{
				component->Remove();
			}

			_GameObjects.erase(gameObject);
		}
	}

	void GameObject::RemoveChildren()
	{
		/* remove all the children if any */
		for (auto& child : _GameObjects)
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
		/*std::unique_ptr<GameObject> tmp = _GameObjects[a];
		children[a] = children[b];
		children[b] = tmp;*/
	}

	IBaseComponent* GameObject::AddComponent(std::unique_ptr<IBaseComponent> component)
	{
		component->Setup();

		components.push_back(std::move(component));

		return components.back().get();
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


	GameObject* GameObject::FindChild(const UID uid)
	{
		for (auto& child : GetGameObjects())
		{
			if (child->uid == uid)
			{
				return child.get();
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

		for (auto& go : _GameObjects)
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

		/* additional component */
		jsonxx::Array componentsArray;
		for (auto& component : components)
		{
			componentsArray << component->ToJson();
		}
		object << "components" << componentsArray;

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

		/* additional components */
		if (object.has<jsonxx::Array>("components") == true)
		{
			for (size_t i = 0; i < object.get<jsonxx::Array>("components").size(); i++)
			{
				jsonxx::Object componentJson = object.get<jsonxx::Array>("components").get<jsonxx::Object>((int)i);
				std::unique_ptr<IBaseComponent> component = SceneComponentFactory::Create((SceneComponentType)componentJson.get<jsonxx::Number>("componentType"), this);
				
				if (component != nullptr)
				{
					component->FromJson(object.get<jsonxx::Array>("components").get<jsonxx::Object>((int)i));
					components.push_back(std::move(component));
				}
			}
		}

		return true;
	}
}