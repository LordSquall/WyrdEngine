#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>

namespace Osiris::Editor
{
	class IPropertiesView
	{
	public:
		virtual void OnPropertyEditorDraw() = 0;
	};
}