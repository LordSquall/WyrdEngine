#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/Vector3Property.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"


namespace Osiris::Editor
{
	class Vector3PropertyView : public ScriptPropertyView
	{
	public:

		virtual void DrawUI();


		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(Vector3PropertyView, Osiris::Vector3Property, "OsirisAPI.Vector3");
	};
}