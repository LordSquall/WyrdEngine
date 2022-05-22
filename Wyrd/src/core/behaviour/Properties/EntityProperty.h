#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API EntityProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object, void* data) override;

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(EntityProperty, void*, 0, "WyrdAPI.Entity");
	};
}