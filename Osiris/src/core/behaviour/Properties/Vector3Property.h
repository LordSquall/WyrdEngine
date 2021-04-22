#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Structures.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Osiris
{
	class OSR_LIBRARY_API Vector3Property : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;
		inline Vector3* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(Vector3Property, Vector3, Vector3(), "OsirisAPI.Vector3");
	};
}