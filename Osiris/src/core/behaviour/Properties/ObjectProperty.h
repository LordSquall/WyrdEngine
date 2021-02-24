#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"



namespace Osiris
{
	class OSR_LIBRARY_API ObjectProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual jsonxx::Object ToJson() override;
		virtual bool FromJson(jsonxx::Object& object) override;

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(ObjectProperty, void*, 0, "System.Object");
	};
}