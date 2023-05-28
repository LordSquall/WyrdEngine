#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "jsonxx.h"
#include "Properties/BaseProp.h"

#include <mono/jit/jit.h>

#define PROPERTY_MONO_FACTORY_REGISTER(Name, typeName)\
PropMonoFactory::Register(\
	#typeName,\
	PropMonoFactory::CreatePropGUI##typeName)


namespace Wyrd
{
	class WYRD_LIBRARY_API PropMonoFactory
	{
	public:
		using SetPropMonoFunc = void(*)(BaseProp*, MonoImage*, const std::string&, MonoObject*);

	public:
		PropMonoFactory() = delete;

		static bool Register(const std::string typeName, SetPropMonoFunc createFunc);

		static void SetProperty(BaseProp* prop, MonoImage* image, const std::string& className, MonoObject* object);

		static std::map<std::string, PropMonoFactory::SetPropMonoFunc>* GetProps();
	};
}