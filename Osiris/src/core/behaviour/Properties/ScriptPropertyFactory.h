#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptProperty.h"

#define SCRIPT_PROPERTY_FACTORY_REGISTER(clsname)\
std::shared_ptr<ScriptProperty> clsname::CreateClone() {\
	std::shared_ptr<clsname> clone = std::make_shared<clsname>();\
	clone->SetName(_Name);\
	clone->SetSetter(_Setter);\
	clone->SetGetter(_Getter);\
	return clone;\
}\
bool clsname::s_Registered = ScriptPropertyFactory::Register(\
clsname::GetFactoryName(),\
clsname::CreateProperty)

#define SCRIPT_PROPERTY_FACTORY_SETUP(clsname, valuetype, defaultValue, factoryname) \
public:\
clsname() : _Value(defaultValue) {}\
virtual std::shared_ptr<ScriptProperty> CreateClone();\
inline virtual const std::string GetTypeName() { return  ##factoryname; };\
static std::unique_ptr<ScriptProperty> CreateProperty() {\
	return std::make_unique<clsname>();\
}\
static std::string GetFactoryName() { return  ##factoryname; };\
private:\
	valuetype _Value;\
	static bool s_Registered

namespace Osiris
{

	class OSR_LIBRARY_API ScriptPropertyFactory
	{
	public:
		using CreatePropertyFunc = std::unique_ptr<ScriptProperty>(*)();

	public:
		ScriptPropertyFactory() = delete;

		static bool Register(const std::string name, CreatePropertyFunc createFunc);

		static std::unique_ptr<ScriptProperty> Create(const std::string& name);
	};
}