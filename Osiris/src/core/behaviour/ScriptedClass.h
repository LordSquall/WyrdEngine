#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class OSR_LIBRARY_API ScriptedClass
	{
	public:
		ScriptedClass(void* domain, const std::string& filename, const std::string& name);
		ScriptedClass(void* assembly, void* image, const std::string& name);
		virtual ~ScriptedClass();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

	public:
		MonoAssembly* Assembly;
		MonoImage* Image;
		MonoClass* Class;

	private:
		std::string _Name;
	};
}