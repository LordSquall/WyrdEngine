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
				const char* typeName = mono_type_get_name(propertyType);

				int type = mono_type_get_type(propertyType);

				std::unique_ptr<ScriptProperty> scriptProp = ScriptPropertyFactory::Create(typeName);
				if (scriptProp != nullptr)
				{
					scriptProp->SetName(name);
					scriptProp->SetSetter(setter);
					scriptProp->SetGetter(getter);

					_Properties.push_back(std::move(scriptProp));
				}
				else
				{
					OSR_WARN("Unable to parse C# property '{0}'. Missing Factory Implementation!", typeName);
				}
			}
			else
			{
				OSR_WARN("Unable to parse C# property '{0}'. Missing Get/Set function!", name);
			}

			///* TESTING */
			//std::unique_ptr<ScriptProperty> prop_test = ScriptPropertyFactory::Create("IntProperty");


			//PropertyDesc propertyDesc = {};
			//propertyDesc.name = mono_property_get_name(prop);
			//propertyDesc.getter = mono_property_get_get_method(prop);
			//propertyDesc.setter = mono_property_get_set_method(prop);

			//if (propertyDesc.getter != nullptr && propertyDesc.setter != nullptr)
			//{
			//	MonoMethodSignature* getterSignature = mono_method_get_signature((MonoMethod*)propertyDesc.getter, 0, 0);
			//	MonoType* propertyType = mono_signature_get_return_type(getterSignature);
			//	const char* name = mono_type_get_name(propertyType);
			//	int type = mono_type_get_type(propertyType);
			//	
			//	/* map the mono type */
			//	switch (static_cast<MonoTypeEnum>(type))
			//	{
			//		case MONO_TYPE_STRING:
			//			propertyDesc.type = PropType::STRING;
			//			break;
			//		case MONO_TYPE_I4:
			//			propertyDesc.type = PropType::INT;
			//			break;
			//		case MONO_TYPE_R4:
			//			propertyDesc.type = PropType::FLOAT;
			//			break;
			//		case MONO_TYPE_CLASS:

			//			// Need to rethink this :/
			//			if (strcmp(name, "OsirisAPI.Texture") == 0)
			//			{
			//				propertyDesc.type = PropType::TEXTURE;
			//				propertyDesc.objectClassNameVal = name;
			//			}
			//			else if (strcmp(name, "OsirisAPI.Color") == 0)
			//			{
			//				propertyDesc.type = PropType::COLOR;
			//				propertyDesc.objectClassNameVal = name;
			//			}
			//			else
			//			{
			//				propertyDesc.type = PropType::GAMEOBJECT;
			//				propertyDesc.objectClassNameVal = name;
			//			}
			//			break;
			//		default:
			//			propertyDesc.type = PropType::UNSUPPORTED;
			//			break;
			//	}

			//	_Properties.push_back(propertyDesc);
			//}
			//else
			//{
			//	OSR_WARN("Unable to parse C# property '{0}'. Missing Get/Set function!", propertyDesc.name);
			//}
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