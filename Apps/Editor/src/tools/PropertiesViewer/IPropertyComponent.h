#pragma once

#include "osrpch.h"
#include "core/export.h"

namespace Osiris::Editor
{
	class IPropertiesComponent
	{
	public:
		virtual void OnPropertyEditorDraw() = 0;
	};
}