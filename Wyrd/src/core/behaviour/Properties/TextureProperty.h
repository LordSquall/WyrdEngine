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
		virtual void Set(void* object) override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;

		inline Texture* GetValue() { return _Value; }
		inline void SetValue(Texture* value) { _Value = value; };

		virtual void Resolve(Scene& scene) override;

		inline UID* GetValueUID() { return &_ValueUID; }
		inline void SetValueUID(UID* uid) { _ValueUID = *uid; }

	private:
		UID _ValueUID;

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(TextureProperty, Texture*, 0, "WyrdAPI.Texture");
	};
}