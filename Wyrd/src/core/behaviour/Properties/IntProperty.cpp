#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/IntProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void IntProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(data);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool IntProperty::ToJson(jsonxx::Object& object, void* data)
	{
		object << "value" << _Value;
		return true;
	}

	bool IntProperty::FromJson(jsonxx::Object& object, void** data)
	{
		int val = (int)object.get<jsonxx::Number>("value", 0);
		memcpy(*data, &val, sizeof(int));

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(IntProperty);
}