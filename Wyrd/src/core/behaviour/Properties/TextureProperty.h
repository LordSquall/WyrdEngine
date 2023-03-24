#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API TextureProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* data) override;
		virtual void Set(void* object, void* data) override;

		inline UID GetValue() { return _Value; }
		inline void SetValue(UID value) { _Value = value; };

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(TextureProperty, UID, UID(), WyrdAPI.Texture);
	};
}