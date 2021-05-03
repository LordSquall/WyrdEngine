#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

namespace jsonxx
{
	class Object;
};

namespace Wyrd
{
	class Scene;

	/**
	 * @brief Base Script Property Class
	 * This class represents a single managed class property
	*/
	class WYRD_LIBRARY_API ScriptProperty
	{
	public:
		ScriptProperty() = default;
		virtual ~ScriptProperty() = default;

		/**
		 * @brief Setter function to pass value into the scripting domain
		 * @param object - managed object
		*/
		virtual void Set(void* object) = 0;

		/**
		 * @brief Serialise the Property to json object
		 * @param object json object
		 * @return 
		*/
		virtual bool ToJson(jsonxx::Object& object) = 0;

		/**
		 * @brief Deserialise a json object
		 * @param object 
		 * @return 
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;

		/**
		 * @brief Copy internal value between properties
		 * @param prop
		*/
		virtual void CopyValue(std::shared_ptr<ScriptProperty> prop) = 0;

		/**
		 * @brief Resolve the property agasint a completely loaded scene model
		 * @return 
		*/
		virtual void Resolve(Scene& scene) { }

		/**
		 * @brief Get the mapped factory type name
		 * @return 
		*/
		virtual const std::string GetFactoryName() = 0;

		/**
		 * @brief Create a Deep copy Clone of the property 
		 * @return 
		*/
		virtual std::shared_ptr<ScriptProperty> CreateClone() = 0;

		/**
		 * @brief Get Property Name
		 * @return Name
		*/
		inline const std::string& GetName() const { return _Name; }

		/**
		 * @brief Set Property Name
		 * @param name 
		*/
		inline void SetName(const std::string name) { _Name = name; }

		/**
		 * @brief Get managed setter function
		 * @return 
		*/
		inline void* GetSetter() const { return _Setter; }

		/**
		 * @brief Set managed setter function
		 * @param setter 
		*/
		inline void SetSetter(void* setter) { _Setter = setter; }

		/**
		 * @brief Get managed getter function
		 * @return 
		*/
		inline void* GetGetter() const { return _Getter; }

		/**
		 * @brief Set managed setter function
		 * @param getter 
		*/
		inline void SetGetter(void* getter) { _Getter = getter; }

		/**
		 * @brief Get managed namespace
		 * @return
		*/
		inline const std::string GetNameSpace() const { return _NameSpace; }

		/**
		 * @brief Set managed namespace
		 * @param setter
		*/
		inline void SetNameSpace(std::string ns) { _NameSpace = ns; }

		/**
		 * @brief Get managed type name
		 * @return
		*/
		inline const std::string GetTypeName() const { return _TypeName; }

		/**
		 * @brief Set managed type name
		 * @param setter
		*/
		inline void SetTypeName(std::string type) { _TypeName = type; }

	protected:
		std::string _NameSpace;
		std::string _TypeName;
		std::string _Name;
		void* _Setter;
		void* _Getter;
	};
}