#pragma once

/* local includes */
#include "support/IconLibrary.h"
#include "views/Toolbars/EditorToolbarBase.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	class PrimaryEditorToolbar : public EditorToolbarBase
	{
	public:
		enum Main2DTool
		{
			Translate,
			Rotate,
			Scale
		};

	public:
		PrimaryEditorToolbar();
		virtual ~PrimaryEditorToolbar() { }

		virtual void OnToolbarRender();

	protected:
		std::shared_ptr<Icon> _2dTranslateIcon;
		std::shared_ptr<Icon> _2dRotateIcon;
		std::shared_ptr<Icon> _2dScaleIcon;

		Main2DTool _SelectedTool;
	};
}