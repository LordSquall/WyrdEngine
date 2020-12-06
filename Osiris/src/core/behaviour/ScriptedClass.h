#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class IBaseComponent;

	class OSR_LIBRARY_API ScriptedClass
	{
	public:
		enum PropType
		{
			INT = 1,
			FLOAT = 2,
			STRING = 3,
			GAMEOBJECT = 4,
			TEXTURE = 5,
			COLOR = 6,
			UNSUPPORTED = 0
		};

		/**
		 * @brief Description of a runtime property of the script
		*/
		struct PropertyDesc
		{
			std::string name;
			void* getter;
			void* setter;
			enum PropType type;

			// Values
			float floatVal;
			int intVal;
			std::string stringVal;
			int objectVal;
			std::string objectNameVal;
			std::string objectClassNameVal;
		};

	public:
		ScriptedClass(const std::string& className, void* managedClass, void* domain);
		virtual ~ScriptedClass();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

		inline const std::vector<PropertyDesc> GetProperties() const { return _Properties; }

	public:
		void* Domain;
		void* ManagedClass;

	private:
		std::string _Name;

		std::vector<PropertyDesc> _Properties;
	};
}