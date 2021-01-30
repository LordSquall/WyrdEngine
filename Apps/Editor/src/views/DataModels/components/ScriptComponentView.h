#pragma once

/* core osiris include */
#include <core/scene/components/ScriptComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"
#include "services/ServiceManager.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class ScriptComponentView : public IPropertiesView
	{
	public:
		ScriptComponentView(ScriptComponent* baseComponent);
		ScriptComponentView(const ScriptComponentView& obj) {}

		void OnPropertyEditorDraw() override;

		const std::string GetName() override { return "Script"; }

	private:
		ScriptComponent* _BaseComponent;

		std::shared_ptr<WorkspaceService> _WorkspaceService;

		std::vector<std::unique_ptr<ScriptPropertyView>> _PropertyViews;
	};
}