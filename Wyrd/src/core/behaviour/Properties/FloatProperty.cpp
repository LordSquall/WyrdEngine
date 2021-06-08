#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/FloatProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void FloatProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(data);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool FloatProperty::ToJson(jsonxx::Object& object, void* data)
	{
		object << "value" << (float*)data;
		return true;
	}

	bool FloatProperty::FromJson(jsonxx::Object& object, void** data)
	{
		float val = (float)object.get<jsonxx::Number>("value", 0.0f);
		memcpy(*data, &val, sizeof(float));
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(FloatProperty);
}