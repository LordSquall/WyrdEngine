#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "ScriptedClass.h"
#include "MonoUtils.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Wyrd
{
	ScriptedClass::ScriptedClass(const std::string& className, MonoClass** managedClass, void* domain) : _Name(className)
	{
		ManagedClass = managedClass;

		MonoProperty* unmanagedProp;
		MonoMethod* unmangedMethod;
		void* propertyIter = NULL;
		void* methodIter = NULL;

		WYRD_TRACE("Mapping Scripted Class: {0}", className);

		/* locate each of the functions with the class */
		while ((unmangedMethod = mono_class_get_methods((MonoClass*)*ManagedClass, &methodIter))) {
			std::string methodName = mono_method_get_name(unmangedMethod);
			
			/* exclude some common functions and property functions */
			bool valueMethod = true;
			std::size_t found = 0;
			found = methodName.find("set_");
			if (found != std::string::npos) valueMethod = false;

			found = methodName.find("get_");
			if (found != std::string::npos) valueMethod = false;

			found = methodName.find(".ctor");
			if (found != std::string::npos) valueMethod = false;

			found = methodName.find(".cctor");
			if (found != std::string::npos) valueMethod = false;

			found = methodName.find("ToString");
			if (found != std::string::npos) valueMethod = false;

			if (valueMethod)
			{
				WYRD_TRACE("- Method: {0}", methodName);
				std::shared_ptr<ScriptedMethod> method = std::make_shared<ScriptedMethod>(unmangedMethod);
				Methods[methodName] = method;
			}
		}

		/* at this point we want to process each of the properties in object */
		while ((unmanagedProp = mono_class_get_properties((MonoClass*)*ManagedClass, &propertyIter))) {

			const char* name = mono_property_get_name(unmanagedProp);
			MonoMethod* getter = mono_property_get_get_method(unmanagedProp);
			MonoMethod* setter = mono_property_get_set_method(unmanagedProp);

			/* we are only concerned with properties with both a setter and getter */
			if (getter != nullptr)
			{
				MonoMethodSignature* getterSignature = mono_method_get_signature((MonoMethod*)getter, 0, 0);
				MonoType* propertyType = mono_signature_get_return_type(getterSignature);
				std::string qualifiedType = mono_type_get_name(propertyType);

				std::string fullTypeNS = "";
				std::string fullTypeName = "";

				/* there is a case were the type comes from 'WyrdGame' namespace */
				std::size_t found = std::string(qualifiedType).find('.');
				if (found != std::string::npos)
				{
					fullTypeNS = qualifiedType.substr(0, found);
					fullTypeName = qualifiedType.substr(found + 1);
				}
				
				if (fullTypeNS == "WyrdGame") {
					qualifiedType = "WyrdGame.ScriptedObject";
				}

				std::unique_ptr<ScriptProperty> scriptProp = ScriptPropertyFactory::Create(qualifiedType);
				if (scriptProp != nullptr)
				{
					scriptProp->SetNameSpace(fullTypeNS);
					scriptProp->SetTypeName(fullTypeName);
					scriptProp->SetName(name);
					scriptProp->SetSetter(setter);
					scriptProp->SetGetter(getter);

					WYRD_TRACE("- Property: {0}", name);
					Properties[name] = std::move(scriptProp);
				}
				else
				{
					WYRD_WARN("Unable to parse C# property '{0}'. Missing Factory Implementation!", qualifiedType);
				}
			}
			else
			{
				WYRD_WARN("Unable to parse C# property '{0}'. Missing Get/Set function!", name);
			}
		}
	}

	std::shared_ptr<PropertyList_t> ScriptedClass::GetPropertiesCopy() const
	{
		std::shared_ptr<PropertyList_t> newPropertyList = std::make_shared<std::map<std::string, std::shared_ptr<ScriptProperty>>>();

		for (auto& prop : Properties)
		{
			(*newPropertyList)[prop.first] = prop.second->CreateClone();
		}

		return newPropertyList;
	}
}