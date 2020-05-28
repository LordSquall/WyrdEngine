#pragma once

/* Core includes */
#include <Osiris.h>

using namespace Osiris;

namespace Osiris::Editor {

	class LuaRef;

	/* Editor Scene Data Model Structure */
	class ScriptedObject
	{
	public:
		/* Constructors */
		ScriptedObject();
		~ScriptedObject() {}

	public:
		std::string name;
	
	private:
		std::shared_ptr<LuaRef> _OnStartFunc;
		std::shared_ptr<LuaRef> _OnUpdateFunc;
		std::shared_ptr<LuaRef> _OnEndFunc;
	};
}