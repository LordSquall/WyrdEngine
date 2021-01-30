#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <jsonxx.h>

namespace Osiris
{
	class Scene;

	class OSR_LIBRARY_API ScriptProperty
	{
	public:
		ScriptProperty() = default;
		virtual ~ScriptProperty() = default;

		virtual void Set(void* object) = 0;

		/* Serialisation functions */
		virtual jsonxx::Object ToJson() = 0;
		virtual bool FromJson(jsonxx::Object& object) = 0;

		virtual void Resolve(Scene& scene) { }

		virtual const std::string GetTypeName() = 0;

		virtual std::shared_ptr<ScriptProperty> CreateClone() = 0;

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string name) { _Name = name; }
		inline void* GetSetter() const { return _Setter; }
		inline void SetSetter(void* setter) { _Setter = setter; }
		inline void* GetGetter() const { return _Getter; }
		inline void SetGetter(void* getter) { _Getter = getter; }

	protected:
		std::string _Name;
		void* _Setter;
		void* _Getter;
	};
}