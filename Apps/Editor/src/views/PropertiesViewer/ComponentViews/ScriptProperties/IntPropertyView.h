#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/IntProperty.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class IntPropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(IntPropertyView, Wyrd::IntProperty, "System.Int32");
	};
}