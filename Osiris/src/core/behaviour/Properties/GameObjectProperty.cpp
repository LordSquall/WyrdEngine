#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/Scene.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/Properties/GameObjectProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void GameObjectProperty::Set(void* object)
	{
		/* check to ensure that we have a valid game object */
		if (_Value != nullptr)
		{
			void* addComponentArgs[1] = { Application::Get().GetBehaviour().GetGameObject(_Value->uid)->Object };

			MonoObject* exception;

			mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &addComponentArgs[0], &exception);

			if (exception != nullptr)
			{
				mono_print_unhandled_exception(exception);
			}
		}
	}

	jsonxx::Object GameObjectProperty::ToJson()
	{
		jsonxx::Object object;
		if (_Value != nullptr)
		{
			object << "value" << _Value->uid.str();
		}

		return object;
	}

	bool GameObjectProperty::FromJson(jsonxx::Object& object)
	{
		_ValueUID = UID(object.get<jsonxx::String>("value", ""));
		return true;
	}

	void GameObjectProperty::Resolve(Scene& scene)
	{
		_Value = &*(scene.FindGameObject(_ValueUID));
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(GameObjectProperty);
}