#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "ScriptedClass.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedClass::ScriptedClass(const std::string& className, void* managedClass, void* domain) : _Name(className), Domain(domain)
	{
		ManagedClass = managedClass;

		MonoProperty* prop;
		void* iter = NULL;

		while ((prop = mono_class_get_properties((MonoClass*)ManagedClass, &iter))) {

			PropertyDesc propertyDesc = {};
			propertyDesc.name = mono_property_get_name(prop);
			propertyDesc.getter = mono_property_get_get_method(prop);
			propertyDesc.setter = mono_property_get_set_method(prop);

			if (propertyDesc.getter != nullptr && propertyDesc.setter != nullptr)
			{
				MonoMethodSignature* getterSignature = mono_method_get_signature((MonoMethod*)propertyDesc.getter, 0, 0);
				MonoType* propertyType = mono_signature_get_return_type(getterSignature);
				
				propertyDesc.type = mono_type_get_type(propertyType);

				// At this point we need to create an instance to get the default property values

				_Properties.push_back(propertyDesc);
			}
			else
			{
				OSR_TRACE("Unable to parse C# property '{0}'. Missing Get/Set function!", propertyDesc.name);
			}
		}
	}

	ScriptedClass::~ScriptedClass()
	{

	}
}