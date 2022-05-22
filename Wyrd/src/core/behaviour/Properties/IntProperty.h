#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API IntProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object, void* data) override;

		inline int* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(IntProperty, int32_t, 0, "System.Int32");
	};
}