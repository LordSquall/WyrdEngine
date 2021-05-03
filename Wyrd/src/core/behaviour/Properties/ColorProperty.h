#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/Structures.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"



namespace Wyrd
{
	class WYRD_LIBRARY_API ColorProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;

		inline Color* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(ColorProperty, Color, Color(), "WyrdAPI.Color");
	};
}