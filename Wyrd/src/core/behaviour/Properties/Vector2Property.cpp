/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/Vector2Property.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void Vector2Property::Set(void* object, void* data)
	{
		Vector2* vector2 = (Vector2*)data;

		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> vector2Class = Application::Get().GetBehaviour().GetClass("Vector2");

		MonoObject* vector2Object = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *vector2Class->ManagedClass);

		MonoProperty* xProperty = mono_class_get_property_from_name((MonoClass*)*vector2Class->ManagedClass, "X");
		MonoProperty* yProperty = mono_class_get_property_from_name((MonoClass*)*vector2Class->ManagedClass, "Y");

		MonoMethod* xPropSetter = mono_property_get_set_method(xProperty);
		MonoMethod* yPropSetter = mono_property_get_set_method(yProperty);

		args.push_back(&vector2->x);
		args.push_back(&vector2->y);

		mono_runtime_invoke(xPropSetter, vector2Object, &args[0], nullptr);
		mono_runtime_invoke(yPropSetter, vector2Object, &args[1], nullptr);

		args.clear();

		args.push_back(vector2Object);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(Vector2Property);
}