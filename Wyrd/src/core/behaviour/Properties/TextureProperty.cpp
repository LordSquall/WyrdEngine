#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/TextureProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <jsonxx.h>

namespace Wyrd
{
	void TextureProperty::Set(void* object, void* data)
	{
		/* cast the texture UID */
		UID textureUID = UID((char*)data);

		/* retrieve the texture pointer for the resource manager */
		auto texturePtr = Application::Get().GetResources().Textures[textureUID];

		std::vector<void*> args;

		std::shared_ptr<ScriptedClass> textureClass = Application::Get().GetBehaviour().GetClass("Texture");

		MonoObject* textureObject = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *textureClass->ManagedClass);

		MonoProperty* property = mono_class_get_property_from_name((MonoClass*)*textureClass->ManagedClass, "NativePtr");

		MonoMethod* propSetter = mono_property_get_set_method(property);

		args.push_back(texturePtr.get());

		mono_runtime_invoke(propSetter, textureObject, &args[0], nullptr);

		args.clear();

		args.push_back(textureObject);

		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}	


	bool TextureProperty::ToJson(jsonxx::Object& object, void* data)
	{
		if (_Value != nullptr)
		{
			object << "value" << _Value->GetUID().str();
		}
		else
		{
			object << "value" << RESOURCE_DEFAULT_TEXTURE;
		}

		return true;
	}

	bool TextureProperty::FromJson(jsonxx::Object& object, void** data)
	{
		_ValueUID = UID(object.get<jsonxx::String>("value", ""));
		return true;
	}

	void TextureProperty::Resolve(Scene& scene)
	{
		_Value = Application::Get().GetResources().Textures[_ValueUID].get();
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(TextureProperty);
}