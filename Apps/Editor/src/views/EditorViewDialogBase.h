#pragma once

/* local includes */
#include "support/IconLibrary.h"
#include "views/EditorViewBase.h"

/* core includes */
#include "core/maths/Rect.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{
	class EditorViewDialogBase : EditorViewBase
	{
	public:
		EditorViewDialogBase(const std::string& name, EditorLayer* editorLayer) : EditorViewBase(name, editorLayer) { _name = name; }
		virtual ~EditorViewDialogBase() = default;

		virtual void OnDialogRender() = 0;

		void Close();

		inline const std::string& GetName() { return _name; }

	private:
		std::string _name;
	};
}