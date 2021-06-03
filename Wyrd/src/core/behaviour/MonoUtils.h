#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	class MonoUtils
	{
	public:

		static bool SetProperty(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& prop, MonoObject* obj, std::vector<void*> value);

		static bool InvokeMethod(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& method, MonoObject* obj, std::vector<void*> values);

	};
}