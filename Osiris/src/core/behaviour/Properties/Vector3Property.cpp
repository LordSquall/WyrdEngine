#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/Vector3Property.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Osiris
{
	void Vector3Property::Set(void* object)
	{
		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> vector3Class = Application::Get().GetBehaviour().GetClass("Vector3");
		MonoObject* vector3Object = MonoUtils::CreateNewObject((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), vector3Class);

		MonoProperty* xProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "X");
		MonoProperty* yProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Y");
		MonoProperty* zProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Z");

		MonoMethod* xPropSetter = mono_property_get_set_method(xProperty);
		MonoMethod* yPropSetter = mono_property_get_set_method(yProperty);
		MonoMethod* zPropSetter = mono_property_get_set_method(zProperty);

		args.push_back(&_Value.x);
		args.push_back(&_Value.y);
		args.push_back(&_Value.z);

		mono_runtime_invoke(xPropSetter, vector3Object, &args[0], nullptr);
		mono_runtime_invoke(yPropSetter, vector3Object, &args[1], nullptr);
		mono_runtime_invoke(zPropSetter, vector3Object, &args[2], nullptr);

		args.clear();

		args.push_back(vector3Object);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	bool Vector3Property::ToJson(jsonxx::Object& object)
	{
		object << "value" << _Value;
		return true;
	}

	bool Vector3Property::FromJson(jsonxx::Object& object)
	{
		_Value << object.get<jsonxx::Array>("value");
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(Vector3Property);
}