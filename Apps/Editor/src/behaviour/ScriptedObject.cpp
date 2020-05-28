#pragma once
#include "osrpch.h"

/* Local includes */
#include "ScriptedObject.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>

namespace Osiris::Editor
{
	ScriptedObject::ScriptedObject()
	{
	}
}