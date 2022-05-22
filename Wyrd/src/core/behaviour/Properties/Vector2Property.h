#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/Structures.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API Vector2Property : public ScriptProperty
	{
	public:
		virtual void Set(void* object, void* data) override;

		inline Vector2* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(Vector2Property, Vector2, Vector2(), "WyrdAPI.Vector2");
	};
}