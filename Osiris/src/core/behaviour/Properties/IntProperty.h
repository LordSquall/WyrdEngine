#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"



namespace Osiris
{
	class OSR_LIBRARY_API IntProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual jsonxx::Object ToJson() override;
		virtual bool FromJson(jsonxx::Object& object) override;


		inline int* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(IntProperty, int32_t, 0, "System.Int32");
	};
}