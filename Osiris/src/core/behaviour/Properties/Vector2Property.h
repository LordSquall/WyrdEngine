#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Structures.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"



namespace Osiris
{
	class OSR_LIBRARY_API Vector2Property : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;
		inline Vector2* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(Vector2Property, Vector2, Vector2(), "OsirisAPI.Vector2");
	};
}