#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/PointerProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void PointerProperty::Set(void* object)
	{

	}

	bool PointerProperty::ToJson(jsonxx::Object& object)
	{
		return true;
	}

	bool PointerProperty::FromJson(jsonxx::Object& object)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(PointerProperty);
}