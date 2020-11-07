#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "ScriptedClass.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedClass::ScriptedClass(const std::string& className, void* managedClass) : _Name(className)
	{
		ManagedClass = managedClass;
	}

	ScriptedClass::~ScriptedClass()
	{

	}
}