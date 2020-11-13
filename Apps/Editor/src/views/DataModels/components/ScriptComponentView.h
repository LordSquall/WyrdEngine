#pragma once

/* core osiris include */
#include <Osiris.h>
#include <core/scene/components/ScriptComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

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

	private:
		ScriptComponent* _BaseComponent;

	};
}