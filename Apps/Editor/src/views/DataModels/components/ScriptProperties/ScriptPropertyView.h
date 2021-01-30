#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

namespace Osiris::Editor
{
	class ScriptPropertyView
	{
	public:
		ScriptPropertyView() = default;
		virtual ~ScriptPropertyView() = default;

		virtual void DrawUI() = 0;
	};
}