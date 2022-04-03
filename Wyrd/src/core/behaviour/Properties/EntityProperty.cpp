#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/ecs/ECS.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/Properties/EntityProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void EntityProperty::Set(void* object, void* data)
	{
		Entity* entity = (Entity*)data;
		auto scriptedEntity = Application::Get().GetBehaviour().ScriptedEntities[*entity];

		std::vector<void*> args;

		args.push_back(scriptedEntity);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool EntityProperty::ToJson(jsonxx::Object& object, void* data)
	{
		return true;
	}

	bool EntityProperty::FromJson(jsonxx::Object& object, void** data)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(EntityProperty);
}