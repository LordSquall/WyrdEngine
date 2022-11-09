/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/Properties/StringProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void StringProperty::Set(void* data)
	{
		_Value = *(std::string*)data;
	}

	void StringProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(mono_string_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), ((std::string*)data)->c_str()));

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(StringProperty);
}