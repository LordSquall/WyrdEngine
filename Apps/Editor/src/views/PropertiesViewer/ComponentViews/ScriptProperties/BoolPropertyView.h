#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/BoolProperty.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class BoolPropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(BoolPropertyView, Wyrd::BoolProperty, "System.Boolean");
	};
}