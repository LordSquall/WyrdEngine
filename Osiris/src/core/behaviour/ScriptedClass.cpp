#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "ScriptedClass.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedClass::ScriptedClass(void* domain, const std::string& filename, const std::string& name) : _Name(name)
	{
		/* Create Assembly */
		MonoAssembly* monoAssembly;
		std::string assemblyPath = (filename.substr(0, filename.find_last_of('.')) + ".dll").c_str();
		monoAssembly = mono_domain_assembly_open((MonoDomain*)domain, assemblyPath.c_str());
		if (!monoAssembly)
		{
			OSR_CORE_ERROR("mono_domain_assembly_open failed for {0}", assemblyPath);
			system("pause");
		}

		/* Create image */
		MonoImage* monoImage = mono_assembly_get_image(monoAssembly);
		if (!monoImage)
		{
			OSR_CORE_ERROR("mono_assembly_get_image failed for {0}", assemblyPath);
			system("pause");
		}

		/* Create class */
		MonoClass* monoClass;
		std::string className = (name);
		monoClass = mono_class_from_name(monoImage, "", className.c_str());

		if (!monoClass)
		{
			OSR_CORE_ERROR("mono_class_from_name failed for {0}", className);
			system("pause");
		}

		Assembly = monoAssembly;
		Image = monoImage;
		Class = monoClass;
	}

	ScriptedClass::ScriptedClass(void* assembly, void* image, const std::string& name) : _Name(name)
	{
		/* Create class */
		MonoClass* monoClass;
		std::string className = (name);
		monoClass = mono_class_from_name((MonoImage*)image, "OsirisAPI", className.c_str());

		if (!monoClass)
		{
			OSR_CORE_ERROR("mono_class_from_name failed for {0}", className);
		}

		Assembly = (MonoAssembly*)assembly;
		Image = (MonoImage*)image;
		Class = monoClass;
	}

	ScriptedClass::~ScriptedClass()
	{

	}
}