#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/behaviour/Properties/ScriptPropertyFactory.h"


namespace Osiris
{
	class OSR_LIBRARY_API ScriptedObjectProperty : public ScriptProperty
	{
	public:
		virtual void Set(void* object) override;

		virtual jsonxx::Object ToJson() override;
		virtual bool FromJson(jsonxx::Object& object) override;

		virtual void Resolve(Scene& scene) override;

		inline GameObject* GetValue() { return _Value; }
		inline void SetValue(GameObject* value) { _Value = value; };

		inline UID* GetValueUID() { return &_ValueUID; }
		inline void SetValueUID(UID* uid) { _ValueUID = *uid; }

	private:
		UID _ValueUID;

	private:
		SCRIPT_PROPERTY_FACTORY_SETUP(ScriptedObjectProperty, GameObject*, 0, "OsirisAPI.ScriptedObject");
	};
}