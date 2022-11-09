#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/Structures.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API Vector3Property : public ScriptProperty
	{
	public:
		virtual void Set(void* data) override;
		virtual void Set(void* object, void* data) override;
		
		inline Vector3* GetValue() { return &_Value; }

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(Vector3Property, Vector3, Vector3(), WyrdAPI.Vector3);
	};
}