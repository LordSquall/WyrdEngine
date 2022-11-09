#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/Properties/TextureProperty.h>

/* local includes */
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class TexturePropertyView;

	class TexturePropertyView
	{
	public:
		SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(TexturePropertyView, Wyrd::TextureProperty, WyrdAPI.Texture);
	};
}