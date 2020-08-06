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
		
	private:
		void CacheResources();

	public:
		std::shared_ptr<ScriptedClass> Class;
		std::shared_ptr<ScriptedCustomObject> Object;
	};
}