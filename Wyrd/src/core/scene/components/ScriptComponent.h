#pragma once

/* local includes */
#include "core/behaviour/ScriptPropertyDesc.h"
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Wyrd {

	class GameObject;
	class ScriptedClass;
	class ScriptedCustomObject;
	class ScriptProperty;

	typedef std::map<std::string, std::shared_ptr<ScriptProperty>> PropertyList_t;

	class WYRD_LIBRARY_API ScriptComponent : public IBaseComponent
	{
	public:
		ScriptComponent(GameObject* gameObject);
		ScriptComponent(const ScriptComponent& obj);
		~ScriptComponent();

		inline virtual const std::string GetManagedType() { return "ScriptComponent"; }
		
		void SetClass(std::shared_ptr<ScriptedClass> scriptedClass);
		void SetCustomObject(std::shared_ptr<ScriptedCustomObject> scriptedObject);

		inline const std::shared_ptr<ScriptedClass> GetClass() const { return _Class; }
		inline const std::shared_ptr<ScriptedCustomObject> GetCustomObject() const { return _Object; }
		
		/**
		 * @brief See IBaseComponent::ToJson()
		*/
		virtual bool ToJson(jsonxx::Object& object) override;

		/**
		 * @brief See IBaseComponent::FromJson()
		*/
		virtual bool FromJson(jsonxx::Object& object) override;

	public:
		std::shared_ptr<PropertyList_t> Properties;

	private:
		void CacheResources();

	private:
		std::shared_ptr<ScriptedClass> _Class;
		std::shared_ptr<ScriptedCustomObject> _Object;
	};
}