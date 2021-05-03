#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptProperty.h"
#include "core/behaviour/ScriptedMethod.h"
#include "core/UID.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	class IBaseComponent;

	typedef std::map<std::string, std::shared_ptr<ScriptProperty>> PropertyList_t;
	typedef std::map<std::string, std::shared_ptr<ScriptedMethod>> MethodList_t;

	/**
	 * @brief Scripted Class
	 * 
	 * Wrapper Class for a 'ManagedClass' object. Builds a map of a managed class structure and provider
	 * and native interface to create and modity managed objects
	*/
	class WYRD_LIBRARY_API ScriptedClass
	{
	public:
		/**
		 * @brief Constructor
		 * @param className		- Name of the class
		 * @param managedClass	- Managed class pointer
		 * @param domain		- Managed Scripting Domain
		*/
		ScriptedClass(const std::string& className, MonoClass** managedClass, void* domain);
		virtual ~ScriptedClass() = default;

		/**
		 * @brief Get the Class UID
		 * @return Class uid
		*/
		inline const UID& GetUID() const { return _UID; }

		/**
		 * @brief Set the Class UID
		 * @param uid
		*/
		inline void SetUID(const UID& uid) { _UID = uid; }

		/**
		 * @brief Get the Class Name
		 * @return Class name
		*/
		inline const std::string& GetName() const { return _Name; }

		/**
		 * @brief Set the Class Name
		 * @param name 
		*/
		inline void SetName(const std::string& name) { _Name = name; }

		std::shared_ptr<PropertyList_t> GetPropertiesCopy() const;

	public:
		PropertyList_t	Properties;
		MethodList_t	Methods;
		MonoClass**		ManagedClass;

	private:
		std::string		_Name;
		UID				_UID;
	};
}