#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/ObjectProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void ObjectProperty::Set(void* object)
	{

	}

	jsonxx::Object ObjectProperty::ToJson()
	{
		jsonxx::Object object;
		return object;
	}

	bool ObjectProperty::FromJson(jsonxx::Object& object)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(ObjectProperty);
}