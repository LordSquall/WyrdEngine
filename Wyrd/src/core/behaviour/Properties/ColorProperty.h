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
		virtual void Set(void* data) override;
		virtual void Set(void* object, void* data) override;

		inline Color* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(ColorProperty, Color, Color(), WyrdAPI.Color);
	};
}