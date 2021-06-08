#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/BoolProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void BoolProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(data);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool BoolProperty::ToJson(jsonxx::Object& object, void* data)
	{
		object << "name" << _Name;
		object << "value" << _Value;
		return true;
	}

	bool BoolProperty::FromJson(jsonxx::Object& object, void** data)
	{
		_Value = object.get<jsonxx::Boolean>("value");

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(BoolProperty);
}