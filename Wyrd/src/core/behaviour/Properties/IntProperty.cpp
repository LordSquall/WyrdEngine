/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/IntProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void IntProperty::Set(void* data)
	{
		_Value = *(int*)data;
	}

	void IntProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(data);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	void IntProperty::Serialise(jsonxx::Object& object)
	{
		object << _Name << _Value;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(IntProperty);
}