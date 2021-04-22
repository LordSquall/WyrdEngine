#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/IntProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Osiris
{
	void IntProperty::Set(void* object)
	{
		std::vector<void*> args;

		args.push_back(&_Value);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool IntProperty::ToJson(jsonxx::Object& object)
	{
		object << "value" << _Value;
		return true;
	}

	bool IntProperty::FromJson(jsonxx::Object& object)
	{
		_Value = (int)object.get<jsonxx::Number>("value", 0);

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(IntProperty);
}