#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "ScriptedClass.h"
#include "MonoUtils.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedClass::ScriptedClass(const std::string& className, void* managedClass, void* domain) : _Name(className), Domain(domain)
	{
		ManagedClass = managedClass;

		MonoProperty* prop;
		void* iter = NULL;

		/* at this point we want to process each of the properties in object */
		while ((prop = mono_class_get_properties((MonoClass*)ManagedClass, &iter))) {

			const char* name = mono_property_get_name(prop);
			MonoMethod* getter = mono_property_get_get_method(prop);
			MonoMethod* setter = mono_property_get_set_method(prop);

			/* we are only concerned with properties with both a setter and getter */
			if (setter != nullptr && getter != nullptr)
			{
				MonoMethodSignature* getterSignature = mono_method_get_signature((MonoMethod*)getter, 0, 0);
				MonoType* propertyType = mono_signature_get_return_type(getterSignature);
				std::string qualifiedType = mono_type_get_name(propertyType);

				std::string fullTypeNS = "";
				std::string fullTypeName = "";

				/* there is a case were the type comes from 'OsirisGame' namespace */
				std::size_t found = std::string(qualifiedType).find('.');
				if (found != std::string::npos)
				{
					fullTypeNS = qualifiedType.substr(0, found);
					fullTypeName = qualifiedType.substr(found + 1);
				}
				
				if (fullTypeNS == "OsirisGame") {
					qualifiedType = "OsirisGame.ScriptedObject";
				}

				std::unique_ptr<ScriptProperty> scriptProp = ScriptPropertyFactory::Create(qualifiedType);
				if (scriptProp != nullptr)
				{
					scriptProp->SetNameSpace(fullTypeNS);
					scriptProp->SetTypeName(fullTypeName);
					scriptProp->SetName(name);
					scriptProp->SetSetter(setter);
					scriptProp->SetGetter(getter);

					_Properties.push_back(std::move(scriptProp));
				}
				else
				{
					OSR_WARN("Unable to parse C# property '{0}'. Missing Factory Implementation!", qualifiedType);
				}
			}
			else
			{
				OSR_WARN("Unable to parse C# property '{0}'. Missing Get/Set function!", name);
			}
		}
	}

	ScriptedClass::~ScriptedClass()
	{

	}


	std::unique_ptr<std::vector<std::shared_ptr<ScriptProperty>>> ScriptedClass::GetPropertiesCopy() const
	{
		std::unique_ptr<std::vector<std::shared_ptr<ScriptProperty>>> newPropertyList = std::make_unique<std::vector<std::shared_ptr<ScriptProperty>>>();

		for (auto& prop : _Properties)
		{
			newPropertyList->push_back(prop->CreateClone());
		}

		return std::move(newPropertyList);
	}
}