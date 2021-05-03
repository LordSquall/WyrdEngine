#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/BoolProperty.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class BoolPropertyView : public ScriptPropertyView
	{
	public:

		virtual void DrawUI();


		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(BoolPropertyView, Wyrd::BoolProperty, "System.Boolean");
	};
}