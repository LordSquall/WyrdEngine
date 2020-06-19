#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>

#include "views/PropertiesViewer/IPropertyComponent.h"
#include "datamodels/resources/ScriptRes.h"
#include "behaviour/ScriptedObject.h"
#include "support/IconLibrary.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class GameObject;

	class ScriptComponent : public IPropertiesComponent
	{
	public:
		ScriptComponent();
		ScriptComponent(std::shared_ptr<GameObject> owner);
		ScriptComponent(const ScriptComponent& obj);
		~ScriptComponent();

		inline std::shared_ptr<ScriptRes> GetScriptResource() const {  return _ScriptResource; };
		void SetScriptResource(std::shared_ptr<ScriptRes> scriptResource);
				
		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;

	private:
		void CacheResources();

	private:
		std::shared_ptr<ScriptRes> _ScriptResource;

		std::shared_ptr<Icon> _RemoveIcon;
	};
}