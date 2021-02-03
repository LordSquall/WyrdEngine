#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/PointerProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void PointerProperty::Set(void* object)
	{

	}

	jsonxx::Object PointerProperty::ToJson()
	{
		jsonxx::Object object;
		return object;
	}

	bool PointerProperty::FromJson(jsonxx::Object& object)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(PointerProperty);
}