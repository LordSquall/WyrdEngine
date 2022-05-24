/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/FloatProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void FloatProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(data);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(FloatProperty);
}