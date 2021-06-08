#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/FloatProperty.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class FloatPropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(FloatPropertyView, Wyrd::FloatProperty, "System.Single");
	};
}