#pragma once

/* local includes */
#include "osrpch.h"
#include "core/Application.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/scene/GameObject.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedCustomObject.h"

#include <jsonxx.h>

namespace Osiris
{
	ScriptComponent::ScriptComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::ScriptComponent)
	{
		Initialised = true;
	}

	ScriptComponent::ScriptComponent(const ScriptComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::ScriptComponent)
	{
		Initialised = true;
	}

	ScriptComponent::~ScriptComponent()
	{}


	void ScriptComponent::SetClass(std::shared_ptr<ScriptedClass> scriptedClass)
	{
		_Class = scriptedClass;

		Properties = _Class->GetPropertiesCopy();
	}

	void ScriptComponent::SetCustomObject(std::shared_ptr<ScriptedCustomObject> scriptedObject)
	{
		_Object = scriptedObject;
	}

	bool ScriptComponent::ToJson(jsonxx::Object& object)
	{
		object << "componentType" << (int)SceneComponentType::ScriptComponent;


		if (_Class != nullptr)
		{
			/* script UID */
			object << "ScriptUID" << GetUID().str();


			/* script properties  */
			jsonxx::Array properties;
			for (auto& prop : *Properties)
			{
				jsonxx::Object propObj;
				prop.second->ToJson(propObj);
				propObj << "name" << prop.second->GetName();

				properties << propObj;
			}
			object << "properties" << properties;
		}

		return true;
	}

	bool ScriptComponent::FromJson(jsonxx::Object& object)
	{
		/* script UID */
		UID scriptUID = UID(object.get<jsonxx::String>("ScriptUID", ""));

		/* configure properties */
		std::shared_ptr<ScriptedClass> cls = Application::Get().GetBehaviour().GetCustomClassByUID(scriptUID);
		if (cls != nullptr)
		{
			SetClass(cls);
			SetUID(scriptUID);

			jsonxx::Array properties = object.get<jsonxx::Array>("properties", jsonxx::Array());
			for (size_t i = 0; i < properties.size(); i++)
			{
				jsonxx::Object propObj = properties.get<jsonxx::Object>((int)i);
				jsonxx::String propName = propObj.get<jsonxx::String>("name", "");

				if (propName != "")
				{
					if ((*Properties).find(propName) != (*Properties).end())
					{
						auto foundPro = (*Properties)[propName];

						(*foundPro).FromJson(propObj);
					}
				}
			}
		}
		return true;
	}
}