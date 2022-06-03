#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/Vector3Property.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class Vector3PropertyView;

	class Vector3PropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(Vector3PropertyView, Wyrd::Vector3Property, "WyrdAPI.Vector3");
	};
}