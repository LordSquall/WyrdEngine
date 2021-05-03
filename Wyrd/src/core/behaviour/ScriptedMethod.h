#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	class IBaseComponent;

	class WYRD_LIBRARY_API ScriptedMethod
	{
	public:
		ScriptedMethod(MonoMethod* method);
		virtual ~ScriptedMethod() = default;

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }
		
		inline MonoMethod* GetManagedMethod() const { return _Method; }

	private:
		std::string _Name;

		MonoMethod* _Method;
	};
}