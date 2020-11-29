#pragma once

/* core osiris include */
#include <Osiris.h>
#include <core/scene/components/ScriptComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "services/ServiceManager.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class ScriptComponentView : public IPropertiesView
	{
	public:
		ScriptComponentView(ScriptComponent* baseComponent) : _BaseComponent(baseComponent) {}
		ScriptComponentView(const ScriptComponentView& obj) {}

		void OnPropertyEditorDraw() override;

	private:
		void DrawIntUI(ScriptedClass::PropertyDesc& prop);
		void DrawFloatUI(ScriptedClass::PropertyDesc& prop);
		void DrawStringUI(ScriptedClass::PropertyDesc& prop);
		void DrawObjectUI(ScriptedClass::PropertyDesc& prop);

	private:
		ScriptComponent* _BaseComponent;

		std::shared_ptr<WorkspaceService> _WorkspaceService;
	};
}