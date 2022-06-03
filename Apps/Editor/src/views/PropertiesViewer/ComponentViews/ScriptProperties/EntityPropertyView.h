#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/EntityProperty.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class EntityPropertyView;

	class EntityPropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(EntityPropertyView, Wyrd::EntityProperty, "WyrdAPI.Entity");
	};
}