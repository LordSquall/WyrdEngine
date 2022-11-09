#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API FloatProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* data) override;
		virtual void Set(void* object, void* data) override;

		inline float* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(FloatProperty, float, 1.0f, System.Single);
	};
}