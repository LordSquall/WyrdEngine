#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "ScriptedClass.h"
#include "ScriptedGameObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedGameObject::ScriptedGameObject(Behaviour* behaviour, std::shared_ptr<ScriptedClass> scriptedClass, std::shared_ptr<GameObject> gameObject)
	{
		/* Store the gameobject */
		_GameObject = gameObject;

		/* Store the class */
		Class = (MonoClass*)scriptedClass->ManagedClass;

		/* Create a new managed object */
		Object = MonoUtils::CreateNewObject((MonoDomain*)behaviour->GetDomain(), scriptedClass);

		/* Set the Native GameObject pointer */
		void* setNativePtrArgs[1] = { &*_GameObject };
		mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), Object, setNativePtrArgs, nullptr);

		for (auto& component : gameObject->components)
		{
			std::shared_ptr<ScriptedClass> componentClass = behaviour->GetClass(component->GetManagedType());
			MonoObject* componentManagedObject = MonoUtils::CreateNewObject((MonoDomain*)behaviour->GetDomain(), componentClass);
			void* addComponentArgs[1] = { componentManagedObject };
			mono_runtime_invoke(behaviour->GetClass("GameObject")->Methods["AddComponent"]->GetManagedMethod(), Object, &addComponentArgs[0], nullptr);
		}
	}

	ScriptedGameObject::~ScriptedGameObject()
	{

	}

	std::shared_ptr<GameObject> const ScriptedGameObject::GetGameObject()
	{
		return _GameObject;
	}
}