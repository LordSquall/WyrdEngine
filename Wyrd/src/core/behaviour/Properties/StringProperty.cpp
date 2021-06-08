#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/Properties/StringProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void StringProperty::Set(void* object, void* data)
	{
		std::vector<void*> args;

		args.push_back(mono_string_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), (char*)data));

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool StringProperty::ToJson(jsonxx::Object& object, void* data)
	{
		object << "value" << _Value;
		return true;
	}

	bool StringProperty::FromJson(jsonxx::Object& object, void** data)
	{
		std::string val = object.get<jsonxx::String>("value", "");
		strcpy((char*)*data, val.c_str());

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(StringProperty);
}