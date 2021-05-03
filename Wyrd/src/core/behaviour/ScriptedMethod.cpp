#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "ScriptedMethod.h"
#include "MonoUtils.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Wyrd
{
	ScriptedMethod::ScriptedMethod(MonoMethod* method)
	{
		_Name = mono_method_get_name(method);

		_Method = method;
	}
}