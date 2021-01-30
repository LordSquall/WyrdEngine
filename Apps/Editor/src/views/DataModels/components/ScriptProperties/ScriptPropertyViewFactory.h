#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/Properties/ScriptProperty.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptPropertyView.h"

#define SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(clsname)\
bool clsname::s_registered = ScriptPropertyViewFactory::Register(\
clsname::GetFactoryName(),\
clsname::CreatePropertyView)


#define SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(clsname, propertytype, factoryname) \
clsname::clsname(const std::shared_ptr<propertytype>& prop) { _Property = prop;  }\
std::shared_ptr<clsname> clsname::CreateClone();\
inline virtual const std::string GetTypeName() { return  ##factoryname; };\
static std::unique_ptr<ScriptPropertyView> CreatePropertyView(const std::shared_ptr<ScriptProperty>& prop) {\
	return std::make_unique<clsname>(std::dynamic_pointer_cast<propertytype>(prop));\
}\
static std::string GetFactoryName() { return  ##factoryname; };\
private:\
	std::shared_ptr<propertytype> _Property;\
	static bool s_registered;

namespace Osiris::Editor
{
	class ScriptPropertyViewFactory
	{
	public:
		using CreatePropertyViewFunc = std::unique_ptr<ScriptPropertyView>(*)(const std::shared_ptr<ScriptProperty>& prop);

	public:
		ScriptPropertyViewFactory() = delete;

		static bool Register(const std::string name, CreatePropertyViewFunc createFunc);

		static std::unique_ptr<ScriptPropertyView> Create(const std::shared_ptr<ScriptProperty>& prop);
	};
}