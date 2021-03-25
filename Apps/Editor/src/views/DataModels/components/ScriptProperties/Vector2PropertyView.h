#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/Vector2Property.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"


namespace Osiris::Editor
{
	class Vector2PropertyView : public ScriptPropertyView
	{
	public:
		virtual void DrawUI();

		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(Vector2PropertyView, Osiris::Vector2Property, "OsirisAPI.Vector2");
	};
}