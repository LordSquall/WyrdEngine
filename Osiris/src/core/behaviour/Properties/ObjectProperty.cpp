#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/ObjectProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Osiris
{
	void ObjectProperty::Set(void* object)
	{

	}

	bool ObjectProperty::ToJson(jsonxx::Object& object)
	{
		return true;
	}

	bool ObjectProperty::FromJson(jsonxx::Object& object)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(ObjectProperty);
}