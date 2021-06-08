#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/Structures.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/ColorProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void ColorProperty::Set(void* object, void* data)
	{
		/* cast data to color structure */
		Color* color = (Color*)data;
		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> colorClass = Application::Get().GetBehaviour().GetClass("Color");
		MonoObject* colorObject = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *colorClass->ManagedClass);
		
		mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *colorClass->ManagedClass);

		/* Call the object default constructor */
		MonoProperty* rProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "R");
		MonoProperty* gProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "G");
		MonoProperty* bProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "B");
		MonoProperty* aProperty = mono_class_get_property_from_name((MonoClass*)*colorClass->ManagedClass, "A");

		MonoMethod* rPropSetter = mono_property_get_set_method(rProperty);
		MonoMethod* gPropSetter = mono_property_get_set_method(gProperty);
		MonoMethod* bPropSetter = mono_property_get_set_method(bProperty);
		MonoMethod* aPropSetter = mono_property_get_set_method(aProperty);

		args.push_back(&color->r);
		args.push_back(&color->g);
		args.push_back(&color->b);
		args.push_back(&color->a);

		mono_runtime_invoke(rPropSetter, colorObject, &args[0], nullptr);
		mono_runtime_invoke(gPropSetter, colorObject, &args[1], nullptr);
		mono_runtime_invoke(bPropSetter, colorObject, &args[2], nullptr);
		mono_runtime_invoke(aPropSetter, colorObject, &args[3], nullptr);

		args.clear();

		args.push_back(colorObject);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool ColorProperty::ToJson(jsonxx::Object& object, void* data)
	{
		/* cast data to color structure */
		Color* color = (Color*)data;
		jsonxx::Array colorArray;

		colorArray << color->r << color->g << color->b << color->a;
		object << "channels" << colorArray;
		return true;
	}

	bool ColorProperty::FromJson(jsonxx::Object& object, void** data)
	{
		Color color;
		jsonxx::Array defaultColor;
		defaultColor << 1.0f << 1.0f << 1.0f << 1.0f;

		jsonxx::Array colorArray = object.get<jsonxx::Array>("channels", defaultColor);
		color.r = (float)colorArray.get<jsonxx::Number>(0);
		color.g = (float)colorArray.get<jsonxx::Number>(1);
		color.b = (float)colorArray.get<jsonxx::Number>(2);
		color.a = (float)colorArray.get<jsonxx::Number>(3);

		memcpy(*data, &color, sizeof(Color));

		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(ColorProperty);
}