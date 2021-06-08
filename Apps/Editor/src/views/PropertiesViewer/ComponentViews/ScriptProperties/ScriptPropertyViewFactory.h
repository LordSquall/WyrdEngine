#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/behaviour/Properties/ScriptProperty.h>

#define SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(clsname)\
bool clsname::s_registered = ScriptPropertyViewFactory::Register(\
clsname::GetFactoryName(),\
clsname::OnEditorRender)


#define SCRIPT_PROPERTY_VIEW_FACTORY_SETUP(clsname, propertytype, factoryname) \
public:\
	static void OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value); \
clsname::clsname(const std::shared_ptr<propertytype>& prop) { _Property = prop;  }\
std::shared_ptr<clsname> clsname::CreateClone();\
inline virtual const std::string GetTypeName() { return  ##factoryname; };\
static std::string GetFactoryName() { return  ##factoryname; };\
private:\
	std::shared_ptr<propertytype> _Property;\
	static bool s_registered;

namespace Wyrd::Editor
{
	class ScriptPropertyViewFactory
	{
	public:
		using CreatePropertyViewFunc = void(*)(const std::shared_ptr<ScriptProperty>& prop, void* value);

	public:
		ScriptPropertyViewFactory() = delete;

		static bool Register(const std::string name, CreatePropertyViewFunc createFunc);

		static bool Create(const std::shared_ptr<ScriptProperty>& prop, void* data);
	};
}