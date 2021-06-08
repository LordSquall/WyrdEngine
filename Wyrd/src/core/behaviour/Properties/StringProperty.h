#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"



namespace Wyrd
{
	class WYRD_LIBRARY_API StringProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object, void* data) override;

		virtual bool ToJson(jsonxx::Object& object, void* data) override;
		virtual bool FromJson(jsonxx::Object& object, void** data) override;

		inline std::string* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(StringProperty, std::string, "", "System.String");
	};
}