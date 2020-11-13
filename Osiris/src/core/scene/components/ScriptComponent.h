#pragma once

/* local includes */
#include "Osiris.h"
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {

	class GameObject;

	class OSR_LIBRARY_API ScriptComponent : public IBaseComponent
	{
	public:
		ScriptComponent(std::shared_ptr<GameObject> owner);
		ScriptComponent(const ScriptComponent& obj);
		~ScriptComponent();
		
		void SetClass(std::shared_ptr<ScriptedClass> scriptedClass);
		void SetCustomObject(std::shared_ptr<ScriptedCustomObject> scriptedObject);

		inline const std::shared_ptr<ScriptedClass> GetClass() const { return _Class; }
		inline const std::shared_ptr<ScriptedCustomObject> GetCustomObject() const { return _Object; }
		
	public:
		std::vector<ScriptedClass::PropertyDesc> Properties;

	private:
		void CacheResources();

	private:
		std::shared_ptr<ScriptedClass> _Class;
		std::shared_ptr<ScriptedCustomObject> _Object;
	};
}