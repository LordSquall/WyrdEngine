#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/behaviour/Properties/TextureProperty.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Osiris
{
	void TextureProperty::Set(void* object)
	{

	}

	jsonxx::Object TextureProperty::ToJson()
	{
		return jsonxx::Object();
	}

	bool TextureProperty::FromJson(jsonxx::Object& object)
	{
		return true;
	}

	SCRIPT_PROPERTY_FACTORY_REGISTER(TextureProperty);
}