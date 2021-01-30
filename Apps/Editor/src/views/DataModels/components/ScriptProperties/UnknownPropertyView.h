#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Osiris::Editor
{
	class UnknownPropertyView : public ScriptPropertyView
	{
	public:

		virtual void DrawUI();


		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(UnknownPropertyView, void, "Unknown");
	};
}