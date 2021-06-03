#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/Vector2Property.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"


namespace Wyrd::Editor
{
	class Vector2PropertyView : public ScriptPropertyView
	{
	public:
		virtual void DrawUI();

		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(Vector2PropertyView, Wyrd::Vector2Property, "WyrdAPI.Vector2");
	};
}