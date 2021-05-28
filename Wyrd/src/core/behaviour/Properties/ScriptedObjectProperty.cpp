#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/Scene.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/ScriptedCustomObject.h"
#include "core/behaviour/Properties/ScriptedObjectProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void ScriptedObjectProperty::Set(void* object)
	{
		if (_Value != nullptr)
		{
			/* retreieve the scripted gameobject */
 			auto scriptedGameObject = Application::Get().GetBehaviour().GetGameObject(_Value->uid);

			if (scriptedGameObject != nullptr)
			{
				auto scriptComponent = scriptedGameObject->GetGameObject()->FindScriptComponent(_TypeName);

				if (scriptComponent != nullptr)
				{
					std::vector<void*> args;
					args.push_back(scriptComponent->GetCustomObject()->Object);
					mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
				}

			}
		}
	}

	bool ScriptedObjectProperty::ToJson(jsonxx::Object& object)
	{
		if (_Value != nullptr)
		{
			object << "value" << _Value->uid.str();
		}
		return true;
	}

	bool ScriptedObjectProperty::FromJson(jsonxx::Object& object)
	{
		_ValueUID = UID(object.get<jsonxx::String>("value", ""));
		return true;
	}

	void ScriptedObjectProperty::Resolve(Scene& scene)
	{
		//_Value = &*(scene.FindGameObject(_ValueUID));
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(ScriptedObjectProperty);
}