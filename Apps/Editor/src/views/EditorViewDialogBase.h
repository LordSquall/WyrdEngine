#pragma once

/* local includes */
#include "support/IconLibrary.h"
#include "views/EditorViewBase.h"

/* core includes */
#include "core/maths/Rect.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	/**
	 * @brief Configuration structure of the editor dialog.
	 * This structure is used to control the common properties of a editor dialog.
	*/
	struct EditorDialogConfig
	{
		bool alwaysCenter = false;
		unsigned int width = 800;
		unsigned int height = 600;
	};

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

		/**
		 * @brief Retrieve the dialog settings used for this dialog
		*/
		inline const EditorDialogConfig& GetConfig() const { return _dialogConfig; }

		/**
		 * @brief Provide a dialog configuration structure
		*/
		inline void SetConfig(const EditorDialogConfig& config) { _dialogConfig = config; }

	protected:
		void* result;
		EditorDialogConfig _dialogConfig;

	private:
		std::string _name;
	};
}