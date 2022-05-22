#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptProperty.h"

#define SCRIPT_PROPERTY_FACTORY_REGISTER(clsname)\
std::shared_ptr<ScriptProperty> clsname::CreateClone() {\
	std::shared_ptr<clsname> clone = std::make_shared<clsname>();\
	clone->SetName(_Name);\
	clone->SetSetter(_Setter);\
	clone->SetGetter(_Getter);\
	clone->SetNameSpace(_NameSpace);\
	clone->SetTypeName(_TypeName);\
	return clone;\
}\
bool clsname::s_Registered = ScriptPropertyFactory::Register(\
clsname::GetManagedType(),\
clsname::CreateProperty)


#define SCRIPT_PROPERTY_FACTORY_SETUP(clsname, valuetype, defaultValue, factoryname) \
public:\
clsname() : _Value(defaultValue) {}\
virtual std::shared_ptr<ScriptProperty> CreateClone();\
inline virtual const std::string GetFactoryName() { return  #factoryname; };\
static std::unique_ptr<ScriptProperty> CreateProperty() {\
	return std::make_unique<clsname>();\
}\
static std::string GetManagedType() { return  #factoryname; };\
public:\
inline virtual void CopyValue(std::shared_ptr<ScriptProperty> prop) { _Value = std::dynamic_pointer_cast<clsname>(prop)->_Value; };\
private:\
	valuetype _Value;\
	static bool s_Registered


namespace Wyrd
{

	class WYRD_LIBRARY_API ScriptPropertyFactory
	{
	public:
		using CreatePropertyFunc = std::unique_ptr<ScriptProperty>(*)();

	public:
		ScriptPropertyFactory() = delete;

		static bool Register(const std::string name, CreatePropertyFunc createFunc);

		static std::unique_ptr<ScriptProperty> Create(const std::string& name);
	};
}