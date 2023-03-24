/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/Properties/TextureProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"
#include "core/behaviour/MonoUtils.h"
#include "serial/TypeSerialisers.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	void TextureProperty::Set(void* data)
	{
		_Value.swap(*(UID*)data);
	}

	void TextureProperty::Set(void* object, void* data)
	{
		/* setup a vector to pass into the natrive pointer property */
		std::vector<void*> args;
		args.push_back(data);

		/* create a new managed object for the texture */
		std::shared_ptr<ScriptedClass> textureClass = Application::Get().GetBehaviour().GetClass("Texture");
		MonoObject* textureObject = mono_object_new((MonoDomain*)Application::Get().GetBehaviour().GetDomain(), *textureClass->ManagedClass);
		
		/* retrieve the mono native property settings function */
		MonoProperty* property = mono_class_get_property_from_name((MonoClass*)*textureClass->ManagedClass, "NativePtr");
		MonoMethod* propSetter = mono_property_get_set_method(property);

		/* invoke the native function to set the property */
		mono_runtime_invoke(propSetter, textureObject, &args[0], nullptr);
		args.clear();

		/* setup the arguments with the managed texture object */
		args.push_back(textureObject);

		/* set the texture on the user script texture property */
		mono_runtime_invoke((MonoMethod*)_Setter, (MonoObject*)object, &args[0], nullptr);
	}

	void TextureProperty::Serialise(jsonxx::Object& object)
	{
		object << _Name << _Value.str();
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(TextureProperty);
}