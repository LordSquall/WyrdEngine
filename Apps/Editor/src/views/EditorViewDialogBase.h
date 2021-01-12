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
	/**
	 * @brief Base class for all Editor Dialog Views
	*/
	class EditorViewDialogBase : EditorViewBase
	{
	public:
		EditorViewDialogBase(const std::string& name, EditorLayer* editorLayer) : EditorViewBase(name, editorLayer) { _name = name; }
		virtual ~EditorViewDialogBase() = default;

		/**
		 * @brief Called on Each Dialog Render function. 
		 * Only called when the dialog is current open
		*/
		virtual void OnDialogRender() = 0;

		/**
		 * @brief Close the dialog
		*/
		void Close();

		/**
		 * @brief Get Name
		 * @return 
		*/
		inline const std::string& GetName() { return _name; }

	protected:
		void* result;

	private:
		std::string _name;
	};
}