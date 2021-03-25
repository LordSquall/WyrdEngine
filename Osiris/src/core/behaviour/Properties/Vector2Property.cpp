#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/Vector2Property.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void Vector2Property::Set(void* object)
	{
	}

	jsonxx::Object Vector2Property::ToJson()
	{
		jsonxx::Object object;
		object << "value" << _Value;
		return object;
	}

	bool Vector2Property::FromJson(jsonxx::Object& object)
	{
		_Value << object.get<jsonxx::Array>("value");
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(Vector2Property);
}