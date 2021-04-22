#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/Properties/StringProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Osiris
{
	void StringProperty::Set(void* object)
	{
		std::vector<void*> args;

		args.push_back(mono_string_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), _Value.c_str()));

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool StringProperty::ToJson(jsonxx::Object& object)
	{
		object << "value" << _Value;
		return true;
	}

	bool StringProperty::FromJson(jsonxx::Object& object)
	{
		_Value = object.get<jsonxx::String>("value", "");

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(StringProperty);
}