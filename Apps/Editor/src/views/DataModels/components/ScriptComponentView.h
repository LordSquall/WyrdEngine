#pragma once

/* core wyrd include */
#include <core/scene/components/ScriptComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"
#include "views/DataModels/components/ScriptProperties/ScriptPropertyViewFactory.h"
#include "services/ServiceManager.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


using namespace Wyrd;
using namespace glm;

namespace Wyrd::Editor {

	class ScriptComponentView : public IPropertiesView
	{
	public:
		ScriptComponentView(ScriptComponent* baseComponent);

		void OnPropertyEditorDraw() override;

		void RebuildPropertyViews();

		const std::string GetName() override { return "Script"; }

	private:
		ScriptComponent* _BaseComponent;

		std::shared_ptr<WorkspaceService> _WorkspaceService;

		std::vector<std::unique_ptr<ScriptPropertyView>> _PropertyViews;
	};
}