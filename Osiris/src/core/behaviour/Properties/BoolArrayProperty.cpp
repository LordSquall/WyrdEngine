#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/BoolArrayProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void BoolArrayProperty::Set(void* object)
	{
		//TODO
	}

	jsonxx::Object BoolArrayProperty::ToJson()
	{
		jsonxx::Object object;
		//TODO
		return object;
	}

	bool BoolArrayProperty::FromJson(jsonxx::Object& object)
	{
		//TODO
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(BoolArrayProperty);
}