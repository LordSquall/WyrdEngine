#pragma once

/* local includes */
#include "osrpch.h"
#include "ScriptComponent.h"
#include "core/scene/GameObject.h"

namespace Osiris
{
	ScriptComponent::ScriptComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::ScriptComponent)
	{
		//OwnerGameObject = owner;

		//CacheResources();
	}

	ScriptComponent::ScriptComponent(const ScriptComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::ScriptComponent)
	{
		//_ScriptResource = obj._ScriptResource;
		//OwnerGameObject = obj.OwnerGameObject;
		
		//CacheResources();
	}

	ScriptComponent::~ScriptComponent()
	{

	}


	//void ScriptComponent::SetScriptResource(std::shared_ptr<ScriptRes> scriptResource)
	//{
	//	/* store the script resource */
	//	_ScriptResource = scriptResource;
	//}
}