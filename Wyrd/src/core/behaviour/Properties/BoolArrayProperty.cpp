#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/BoolArrayProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void BoolArrayProperty::Set(void* object, void* data)
	{
		//TODO
	}

	//bool BoolArrayProperty::ToJson(jsonxx::Object& object, void* data)
	//{
	//	//TODO
	//	return true;
	//}
//
	//bool BoolArrayProperty::FromJson(jsonxx::Object& object, void** data)
	//{
	//	//TODO
	//	return true;
	//}

	SCRIPT_PROPERTY_FACTORY_REGISTER(BoolArrayProperty);
}