#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "views/EditorViewDialogBase.h"

#include "views/Dialogs/ConfirmationDialog.h"

namespace Osiris::Editor
{
	class EditorLayer;

	class DialogService : public IService
	{
	public:
		DialogService() {}
		~DialogService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnGUI() override;

		void OpenDialog(std::shared_ptr<EditorViewDialogBase> dialog);

		void OpenConfirmDialog(EditorLayer* editorLayer, const std::string& message, std::function<void(void*)> successCallback, std::function<void(void*)> failureCallback = nullptr, void* dialogData = nullptr);

	private:
		std::shared_ptr<ConfirmationDialog> _confirmationDialog;

		static std::shared_ptr<EditorViewDialogBase> _activeDialog;
		static bool(*_popupDialogCallback)(std::shared_ptr<EditorViewDialogBase>);
	};
}