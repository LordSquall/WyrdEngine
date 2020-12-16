#pragma once

/* local includes */
#include "osrpch.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/scene/GameObject.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedCustomObject.h"

namespace Osiris
{
	ScriptComponent::ScriptComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::ScriptComponent)
	{}

	ScriptComponent::ScriptComponent(const ScriptComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::ScriptComponent)
	{}

	ScriptComponent::~ScriptComponent()
	{}


	void ScriptComponent::SetClass(std::shared_ptr<ScriptedClass> scriptedClass)
	{
		_Class = scriptedClass;
		Properties = _Class->GetProperties();
	}

	void ScriptComponent::SetCustomObject(std::shared_ptr<ScriptedCustomObject> scriptedObject)
	{
		_Object = scriptedObject;
	}
}