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
	ScriptedGameObject::ScriptedGameObject(void* domain, std::shared_ptr<ScriptedClass> scriptedClass, std::shared_ptr<GameObject> gameObject)
	{
		/* Store the gameobject */
		_GameObject = gameObject;

		/* Store the class */
		Class = (MonoClass*)scriptedClass->ManagedClass;

		/* Get a reference to the Behaviour subsystem */
		Osiris::Behaviour& behaviour = Application::Get().GetBehaviour();

		/* build property arguments */
		UID uid = gameObject->uid;
		void* createMethodArgs[1];
		createMethodArgs[0] = &uid;

		Object = mono_runtime_invoke(behaviour.GetClass("GameObjectManager")->Methods["RegisterGameObject"]->GetManagedMethod(), NULL, createMethodArgs, NULL);

		/* build property arguments */
		Osiris::GameObject* go = &*gameObject;
		void* linkMethodArgs[2];
		linkMethodArgs[0] = &uid;
		linkMethodArgs[1] = &go;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(behaviour.GetClass("GameObjectManager")->Methods["LinkToManaged"]->GetManagedMethod(), &*Object, linkMethodArgs, &exception);
		
		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
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