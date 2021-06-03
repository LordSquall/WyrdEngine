#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

namespace Wyrd::Editor
{
	class ScriptPropertyView
	{
	public:
		ScriptPropertyView() = default;
		virtual ~ScriptPropertyView() = default;

		virtual void DrawUI() = 0;
	};
}