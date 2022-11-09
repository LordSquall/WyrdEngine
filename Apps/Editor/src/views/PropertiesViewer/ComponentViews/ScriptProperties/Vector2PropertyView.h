#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/Vector2Property.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class Vector2PropertyView;

	class Vector2PropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(Vector2PropertyView, Wyrd::Vector2Property, WyrdAPI.Vector2);
	};
}