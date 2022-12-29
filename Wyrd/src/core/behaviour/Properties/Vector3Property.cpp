/* Local includes */
#include "wyrdpch.h"
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

namespace Wyrd
{
	void Vector3Property::Set(void* data)
	{
		_Value.x = ((Vector3*)data)->x;
		_Value.y = ((Vector3*)data)->y;
		_Value.z = ((Vector3*)data)->z;
	}

	void Vector3Property::Set(void* object, void* data)
	{
		Vector3* vector3 = (Vector3*)data;

		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> vector3Class = Application::Get().GetBehaviour().GetClass("Vector3");
		MonoObject* vector3Object = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *vector3Class->ManagedClass);

		MonoProperty* xProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "X");
		MonoProperty* yProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Y");
		MonoProperty* zProperty = mono_class_get_property_from_name((MonoClass*)*vector3Class->ManagedClass, "Z");

		MonoMethod* xPropSetter = mono_property_get_set_method(xProperty);
		MonoMethod* yPropSetter = mono_property_get_set_method(yProperty);
		MonoMethod* zPropSetter = mono_property_get_set_method(zProperty);

		args.push_back(&vector3->x);
		args.push_back(&vector3->y);
		args.push_back(&vector3->z);

		mono_runtime_invoke(xPropSetter, vector3Object, &args[0], nullptr);
		mono_runtime_invoke(yPropSetter, vector3Object, &args[1], nullptr);
		mono_runtime_invoke(zPropSetter, vector3Object, &args[2], nullptr);

		args.clear();

		args.push_back(vector3Object);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}
	
	void Vector3Property::Serialise(jsonxx::Object& object)
	{
		object << _Name << _Value;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(Vector3Property);
}