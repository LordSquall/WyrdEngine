#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/ScriptProperty.h>


/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class UnknownPropertyView : public ScriptPropertyView
	{
	public:
		UnknownPropertyView::UnknownPropertyView(const std::shared_ptr<void>& prop, const std::string name) { _Property = prop; _name = name; }

		virtual void DrawUI();


		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(UnknownPropertyView, void, "Unknown");

	private:
		std::string _name;
	};
}