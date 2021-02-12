#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/ColorProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void ColorProperty::Set(void* object)
	{
		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> colorClass = Application::Get().GetBehaviour().GetClass("Color");
		MonoObject* colorObject = MonoUtils::CreateNewObject((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), colorClass);

		MonoProperty* rProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "R");
		MonoProperty* gProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "G");
		MonoProperty* bProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "B");
		MonoProperty* aProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "A");

		MonoMethod* rPropSetter = mono_property_get_set_method(rProperty);
		MonoMethod* gPropSetter = mono_property_get_set_method(gProperty);
		MonoMethod* bPropSetter = mono_property_get_set_method(bProperty);
		MonoMethod* aPropSetter = mono_property_get_set_method(aProperty);

		args.push_back(&_Value.r);
		args.push_back(&_Value.g);
		args.push_back(&_Value.b);
		args.push_back(&_Value.a);

		mono_runtime_invoke(rPropSetter, colorObject, &args[0], nullptr);
		mono_runtime_invoke(gPropSetter, colorObject, &args[1], nullptr);
		mono_runtime_invoke(bPropSetter, colorObject, &args[2], nullptr);
		mono_runtime_invoke(aPropSetter, colorObject, &args[3], nullptr);

		args.clear();

		args.push_back(colorObject);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	jsonxx::Object ColorProperty::ToJson()
	{
		jsonxx::Object object;
		jsonxx::Array colorArray;
		colorArray << _Value.r << _Value.g << _Value.b << _Value.a;
		object << "channels" << colorArray;

		return object;
	}

	bool ColorProperty::FromJson(jsonxx::Object& object)
	{
		jsonxx::Array defaultColor;
		defaultColor << 1.0f << 1.0f << 1.0f << 1.0f;

		jsonxx::Array colorArray = object.get<jsonxx::Array>("channels", defaultColor);
		_Value.r = (float)colorArray.get<jsonxx::Number>(0);
		_Value.g = (float)colorArray.get<jsonxx::Number>(1);
		_Value.b = (float)colorArray.get<jsonxx::Number>(2);
		_Value.a = (float)colorArray.get<jsonxx::Number>(3);

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(ColorProperty);
}