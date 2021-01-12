#pragma once

#include "osrpch.h"
#include "ServiceManager.h"
#include "DialogService.h"
#include "events/EditorEvents.h"
#include "support/Utils.h"
#include "platform/OpenGL/imgui_opengl_renderer.h"

namespace Osiris::Editor
{
	std::shared_ptr<EditorViewDialogBase> DialogService::_activeDialog = nullptr;
	bool(*DialogService::_popupDialogCallback)(std::shared_ptr<EditorViewDialogBase>) = [](std::shared_ptr<EditorViewDialogBase> dialog)->bool { return false; };

	void DialogService::OnCreate() {}

	void DialogService::OnDestroy() {}

	void DialogService::OnGUI() 
	{
		if (_activeDialog != nullptr)
		{
			bool isOpen = _popupDialogCallback(_activeDialog);

			if (!isOpen)
				_activeDialog = nullptr;
		}
	}

	void DialogService::OpenDialog(std::shared_ptr<EditorViewDialogBase> dialog)
	{
		_activeDialog = dialog;

		_popupDialogCallback = [](std::shared_ptr<EditorViewDialogBase> dialog)->bool
		{
			if (dialog != nullptr)
			{
				const char* name = dialog->GetName().c_str();
				if (!ImGui::IsPopupOpen(name))
					ImGui::OpenPopup(name);

				if (ImGui::BeginPopupModal(name))
				{
					dialog->OnDialogRender();
					ImGui::EndPopup();
				}

				return ImGui::IsPopupOpen(name);
			}
			return false;
		};
	}


	void DialogService::OpenConfirmDialog(EditorLayer* editorLayer, const std::string& message, std::function<void(void*)> successCallback, std::function<void(void*)> failureCallback, void* dialogData)
	{
		if (!_confirmationDialog)
			_confirmationDialog = std::make_shared<ConfirmationDialog>(editorLayer);

		_confirmationDialog->SetMessage(message);
		_confirmationDialog->SetSuccessCallback(successCallback);
		_confirmationDialog->SetFailureCallback(failureCallback);
		_confirmationDialog->SetDialogData(dialogData);

		_activeDialog = _confirmationDialog;

		_popupDialogCallback = [](std::shared_ptr<EditorViewDialogBase> dialog)->bool
		{
			if (dialog != nullptr)
			{
				const char* name = dialog->GetName().c_str();
				if (!ImGui::IsPopupOpen(name))
					ImGui::OpenPopup(name);

				if (ImGui::BeginPopupModal(name, nullptr, ImGuiWindowFlags_NoResize))
				{
					dialog->OnDialogRender();
					ImGui::EndPopup();
				}

				return ImGui::IsPopupOpen(name);
			}
			return false;
		};
	}
}
