#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/StringProperty.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class StringPropertyView : public ScriptPropertyView
	{
	public:

		virtual void DrawUI();

		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(StringPropertyView, Wyrd::StringProperty, "System.String");
	};
}