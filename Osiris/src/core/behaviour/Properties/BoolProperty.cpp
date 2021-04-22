#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/BoolProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Osiris
{
	void BoolProperty::Set(void* object)
	{
		std::vector<void*> args;

		args.push_back(&_Value);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool BoolProperty::ToJson(jsonxx::Object& object)
	{
		object << "name" << _Name;
		object << "value" << _Value;
		return true;
	}

	bool BoolProperty::FromJson(jsonxx::Object& object)
	{
		_Value = object.get<jsonxx::Boolean>("value");

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(BoolProperty);
}