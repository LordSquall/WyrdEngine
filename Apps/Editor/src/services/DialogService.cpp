#include "wyrdpch.h"
#include "ServiceManager.h"
#include "DialogService.h"
#include "events/EditorEvents.h"
#include "support/Utils.h"
#include "platform/OpenGL/imgui_opengl_renderer.h"

#include <core/Application.h>
#include <core/Window.h>

namespace Wyrd::Editor
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
				EditorDialogConfig dialogConfig = dialog->GetConfig();

				const char* title = dialog->GetTitle().c_str();
				if (!ImGui::IsPopupOpen(title))
				{
					if (dialogConfig.alwaysCenter)
					{
						unsigned int currentWinWidth = Application::Get().GetWindow().GetWidth();
						unsigned int currentWinHeight = Application::Get().GetWindow().GetHeight();

						float xPos = (((float)currentWinWidth) * 0.5f) - ((float)dialogConfig.width * 0.5f);
						float yPos = (((float)currentWinHeight) * 0.5f) - ((float)dialogConfig.height * 0.5f);

						ImGui::SetNextWindowPos(ImVec2(xPos, yPos));
						ImGui::SetNextWindowSize(ImVec2((float)dialogConfig.width, (float)dialogConfig.height));
					}
					ImGui::OpenPopup(title);
				}

				if (ImGui::BeginPopupModal(title, nullptr))
				{
					dialog->OnDialogRender();
					ImGui::EndPopup();
				}

				return ImGui::IsPopupOpen(title);
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
				const char* title = dialog->GetTitle().c_str();
				if (!ImGui::IsPopupOpen(title))
					ImGui::OpenPopup(title);

				if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_NoResize))
				{
					dialog->OnDialogRender();
					ImGui::EndPopup();
				}

				return ImGui::IsPopupOpen(title);
			}
			return false;
		};
	}

	void DialogService::OpenSingleEntryDialog(EditorLayer* editorLayer, const std::string title, const std::string& prompt, std::function<void(std::string)> successCallback, std::function<void(std::string)> failureCallback)
	{
		if (!_singleEntryDialog)
			_singleEntryDialog = std::make_shared<SingleEntryDialog>(editorLayer);

		_singleEntryDialog->SetTitle(title);
		_singleEntryDialog->SetPrompt(prompt);
		_singleEntryDialog->SetSuccessCallback(successCallback);
		_singleEntryDialog->SetFailureCallback(failureCallback);

		_activeDialog = _singleEntryDialog;

		_popupDialogCallback = [](std::shared_ptr<EditorViewDialogBase> dialog)->bool
		{
			if (dialog != nullptr)
			{
				const char* name = dialog->GetTitle().c_str();
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
