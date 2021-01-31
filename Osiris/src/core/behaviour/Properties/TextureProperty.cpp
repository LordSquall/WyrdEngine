#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/TextureProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	void TextureProperty::Set(void* object)
	{
		std::vector<void*> args;

		MonoClass* textureClass = (MonoClass*)Application::Get().GetBehaviour().GetClass("Texture")->ManagedClass;
		MonoObject* textureObject = MonoUtils::CreateNewObject("Texture");

		MonoProperty* property = mono_class_get_property_from_name(textureClass, "NativePtr");

		MonoMethod* propSetter = mono_property_get_set_method(property);

		args.push_back(&_Value);

		mono_runtime_invoke(propSetter, textureObject, &args[0], nullptr);

		args.clear();

		args.push_back(textureObject);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
		
	}

	jsonxx::Object TextureProperty::ToJson()
	{
		jsonxx::Object object;
		if (_Value != nullptr)
		{
			object << "value" << _Value->GetUID().str();
		}
		else
		{
			object << "value" << RESOURCE_DEFAULT_TEXTURE;
		}

		return object;
	}

	bool TextureProperty::FromJson(jsonxx::Object& object)
	{
		_ValueUID = UID(object.get<jsonxx::String>("value", ""));
		return true;
	}

	void TextureProperty::Resolve(Scene& scene)
	{
		_Value = Application::Get().GetResources().Textures[_ValueUID];
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(TextureProperty);
}