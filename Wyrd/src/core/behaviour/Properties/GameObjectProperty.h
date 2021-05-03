#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/scene/GameObject.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"

namespace Wyrd
{
	class WYRD_LIBRARY_API GameObjectProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual bool ToJson(jsonxx::Object& object) override;
		virtual bool FromJson(jsonxx::Object& object) override;

		virtual void Resolve(Scene& scene) override;

		inline GameObject* GetValue() { return _Value; }
		inline void SetValue(GameObject* value) { _Value = value; };

		inline UID* GetValueUID() { return &_ValueUID; }
		inline void SetValueUID(UID* uid) { _ValueUID = *uid; }

	private:
		UID _ValueUID;

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(GameObjectProperty, GameObject*, nullptr, "WyrdAPI.GameObject");
	};
}