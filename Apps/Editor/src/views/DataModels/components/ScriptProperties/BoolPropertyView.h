#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/BoolProperty.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Osiris::Editor
{
	class BoolPropertyView : public ScriptPropertyView
	{
	public:

		virtual void DrawUI();


		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(BoolPropertyView, Osiris::BoolProperty, "System.Boolean");
	};
}