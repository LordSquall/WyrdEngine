#pragma once

/* Core includes */
#include <Osiris.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>

using namespace Osiris;

namespace Osiris::Editor {
	
	/* Editor Scene Data Model Structure */
	class ScriptedObjectTemplate
	{
	public:
		/* Constructors */
		ScriptedObjectTemplate();
		~ScriptedObjectTemplate() {}

	public:
		std::shared_ptr<luabridge::LuaRef> OnStartFunc;
		std::shared_ptr<luabridge::LuaRef> OnUpdateFunc;
	};
}