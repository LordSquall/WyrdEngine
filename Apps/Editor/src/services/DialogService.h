#pragma once

#include "core/export.h"

#include "services/ServiceTypes.h"
#include "services/IService.h"

#include "views/EditorViewDialogBase.h"

#include "views/Dialogs/ConfirmationDialog.h"
#include "views/Dialogs/SingleEntryDialog.h"
#include "views/Dialogs/NewShaderStageDialog.h"

namespace Wyrd::Editor
{
	class EditorLayer;

	class DialogService : public IService
	{
	public:
		DialogService() : IsDialogOpen(false) {}
		~DialogService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnGUI() override;

		static ServiceType GetTypeID() { return ServiceType::Dialog; }

		void OpenDialog(std::shared_ptr<EditorViewDialogBase> dialog);

		void OpenConfirmDialog(EditorLayer* editorLayer, const std::string& message, std::function<void(void*)> successCallback, std::function<void(void*)> failureCallback = nullptr, void* dialogData = nullptr);
		void OpenSingleEntryDialog(EditorLayer* editorLayer, const std::string title, const std::string& prompt, std::function<void(std::string)> successCallback, std::function<void(std::string)> failureCallback = nullptr);
		void OpenCreateShaderStageDialog(EditorLayer* editorLayer, const std::string title, const std::string& prompt, std::function<void(std::string, int)> successCallback, std::function<void(std::string, int)> failureCallback = nullptr);

		bool IsDialogOpen;
	private:
		std::shared_ptr<ConfirmationDialog> _confirmationDialog;
		std::shared_ptr<SingleEntryDialog> _singleEntryDialog;
		std::shared_ptr<NewShaderStageDialog> _newShaderStageDialog;

		static std::shared_ptr<EditorViewDialogBase> _activeDialog;
		static bool(*_popupDialogCallback)(std::shared_ptr<EditorViewDialogBase>);

	};
}