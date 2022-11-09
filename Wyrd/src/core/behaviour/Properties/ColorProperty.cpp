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
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void ColorProperty::Set(void* data)
	{
		_Value.r = ((Color*)data)->r;
		_Value.g = ((Color*)data)->g;
		_Value.b = ((Color*)data)->b;
		_Value.a = ((Color*)data)->a;
	}

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


	std::shared_ptr<ScriptProperty> ColorProperty::CreateClone() {
		std::shared_ptr<ColorProperty> clone = std::make_shared<ColorProperty>();
		clone->SetName(_Name);
		clone->SetSetter(_Setter);
		clone->SetGetter(_Getter);
		clone->SetNameSpace(_NameSpace);
		clone->SetTypeName(_TypeName);
		return clone;
	}

	void ColorProperty::Serialise(jsonxx::Object& object) {
		object << _Name << _Value;
	}

	bool ColorProperty::s_Registered = ScriptPropertyFactory::Register(ColorProperty::GetManagedType(), ColorProperty::CreateProperty);

	//SCRIPT_PROPERTY_FACTORY_REGISTER(ColorProperty);
}