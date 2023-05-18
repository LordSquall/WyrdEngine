#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "jsonxx.h"
#include "support/ImGuiUtils.h"

#define PROPERTY_GUI_FACTORY_REGISTER(Name, typeName)\
PropGUIFactory::Register(\
	#typeName,\
	PropGUIFactory::CreatePropGUI##typeName)


namespace Wyrd
{
	class WYRD_LIBRARY_API PropGUIFactory
	{
	public:
		using DrawPropGUIFunc = void(*)(BaseProp*);

	public:
		PropGUIFactory() = delete;

		static bool Register(const std::string typeName, DrawPropGUIFunc createFunc);

		static void DrawProp(BaseProp* prop);

		static void DrawMissingProp(const std::string& name);

		static std::map<std::string, PropGUIFactory::DrawPropGUIFunc>* GetPropsGUI();
	};
}