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
	ScriptedGameObject::ScriptedGameObject(Behaviour* behaviour, std::shared_ptr<ScriptedClass> scriptedClass, std::shared_ptr<GameObject> gameObject, void* managedObject)
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
		void* setNativePtrArgs[1] = { &*_GameObject };
 		mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), Object, &setNativePtrArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		/* Add the transform2D component */
		gameObject->transform2D->ManagedObject = MonoUtils::CreateNewObject((MonoDomain*)behaviour->GetDomain(), behaviour->GetClass("Transform2D"));

		void* setTranform2DComponentNativePtrArgs[1] = { &_GameObject->transform2D };
		mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), (MonoObject*)gameObject->transform2D->ManagedObject, &setTranform2DComponentNativePtrArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		void* addTransform2DComponentArgs[1] = { (MonoObject*)gameObject->transform2D->ManagedObject };
		mono_runtime_invoke(behaviour->GetClass("GameObject")->Methods["AddComponent"]->GetManagedMethod(), Object, &addTransform2DComponentArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		for (auto& component : _GameObject->components)
		{
			std::shared_ptr<ScriptedClass> componentClass = behaviour->GetClass(component->GetManagedType());
			MonoObject* componentManagedObject = MonoUtils::CreateNewObject((MonoDomain*)behaviour->GetDomain(), componentClass);
			
			component->ManagedObject = componentManagedObject;

			void* setComponentNativePtrArgs[1] = { &*component };
			mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), componentManagedObject, &setComponentNativePtrArgs[0], &exception);
			if (exception != nullptr) mono_print_unhandled_exception(exception);

			void* addComponentArgs[1] = { componentManagedObject };
			mono_runtime_invoke(behaviour->GetClass("GameObject")->Methods["AddComponent"]->GetManagedMethod(), Object, &addComponentArgs[0], &exception);
			if (exception != nullptr) mono_print_unhandled_exception(exception);
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