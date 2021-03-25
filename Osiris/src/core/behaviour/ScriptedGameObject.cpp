#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "ScriptedClass.h"
#include "ScriptedGameObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedGameObject::ScriptedGameObject(Behaviour* behaviour, std::shared_ptr<ScriptedClass> scriptedClass, GameObject* gameObject, void* managedObject)
	{
		/* Store the gameobject */
 		_GameObject = gameObject;

		/* Store the class */
		Class = (MonoClass*)*scriptedClass->ManagedClass;

		/* Create a new managed object */
		if (managedObject == nullptr)
			Object = MonoUtils::CreateNewObject((MonoDomain*)behaviour->GetDomain(), scriptedClass);
		else
			Object = (MonoObject*)managedObject;

		MonoObject* exception = nullptr;

		/* Set the Native GameObject pointer */
		void* setNativePtrArgs[1] = { &_GameObject };
 		mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), Object, &setNativePtrArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		/* store the native pointer to the transform component */
		_NativePtrMap.push_back(_GameObject->transform.get());

		/* create the managed object for transform component */
		_GameObject->transform->ManagedObject = MonoUtils::CreateNewUnmanagedObject((MonoDomain*)behaviour->GetDomain(), behaviour->GetClass("Transform2D"), behaviour, &_NativePtrMap[_NativePtrMap.size() - 1]);

		/* add the transform component to the managed game object */
		void* addTranform2DComponentComponentArgs[1] = { _GameObject->transform->ManagedObject };
		mono_runtime_invoke(behaviour->GetClass("GameObject")->Methods["AddComponent"]->GetManagedMethod(), Object, &addTranform2DComponentComponentArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);
		
		for (auto& component : _GameObject->components)
		{
			/* store the native pointer to the component */
			_NativePtrMap.push_back(component.get());

			/* create the managed object for component */
			component->ManagedObject = MonoUtils::CreateNewUnmanagedObject((MonoDomain*)behaviour->GetDomain(), behaviour->GetClass(component->GetManagedType()), behaviour, &_NativePtrMap[_NativePtrMap.size() - 1]);

			void* addComponentArgs[1] = { component->ManagedObject };
			mono_runtime_invoke(behaviour->GetClass("GameObject")->Methods["AddComponent"]->GetManagedMethod(), Object, &addComponentArgs[0], &exception);
			if (exception != nullptr) mono_print_unhandled_exception(exception);
		}
	}

	ScriptedGameObject::~ScriptedGameObject()
	{

	}

	GameObject* ScriptedGameObject::GetGameObject()
	{
		return _GameObject;
	}
}