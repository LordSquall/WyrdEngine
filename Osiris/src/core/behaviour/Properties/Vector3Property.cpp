#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/Vector3Property.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void Vector3Property::Set(void* object)
	{
	}

	jsonxx::Object Vector3Property::ToJson()
	{
		jsonxx::Object object;
		object << "value" << _Value;
		return object;
	}

	bool Vector3Property::FromJson(jsonxx::Object& object)
	{
		_Value << object.get<jsonxx::Array>("value");
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(Vector3Property);
}