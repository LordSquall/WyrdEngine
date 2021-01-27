#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "PreferencesDialog.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	PreferencesDialog::PreferencesDialog(EditorLayer* editorLayer) : EditorViewDialogBase("Preferences Viewer", editorLayer)
	{
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Service::Settings);
	}

	PreferencesDialog::~PreferencesDialog() {}

	void PreferencesDialog::OnDialogRender()
	{
		float navigationPanelWidth = 100.0f;
		float contentPanelHeight = ImGui::GetTextLineHeight() + 10.0f;

		static std::function<void()> subMenu = std::bind(&PreferencesDialog::OnEditorRender_General, this);

		ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y - contentPanelHeight));
		
		if (ImGui::Button("General", ImVec2(navigationPanelWidth, 0))) 
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_General, this);

		if (ImGui::Button("RenderDoc", ImVec2(navigationPanelWidth, 0)))
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_RenderDoc, this);

		ImGui::EndChildFrame();

		ImGui::SameLine();
		ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - contentPanelHeight));
		subMenu();
		ImGui::EndChildFrame();

		/* control buttons */
		if (ImGui::Button("Cancel"))
		{
			Close();
		}

		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			Close();
		}
	}

	void PreferencesDialog::OnEditorRender_General()
	{
		if (ImGui::Button("TEST") == true)
		{

		}
	}

	void PreferencesDialog::OnEditorRender_RenderDoc()
	{
#ifdef OSR_RENDERDOC_ENABLED
		static bool enableRenderDoc = Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__ENABLED, std::string("0")));
		static bool enableOverlay = Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__DISPLAYOVERLAY, std::string("0")));
		static bool autoOpenOnCapture = Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__AUTOOPEN, std::string("0")));
		static std::string captureDir = _SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__AUTOOPEN, std::string("my_captures/example"));
		static int currentKey;

		ImGui::Checkbox("Enable Renderdoc", &enableRenderDoc);
		ImGui::Checkbox("Enable Overlay", &enableOverlay);
		ImGui::Checkbox("Auto-Open on Capture", &autoOpenOnCapture);
		ImGui::InputText("Capture Directory", &captureDir);
		ImGui::Text("Current Render Capture Key: F4");

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(std::to_string(enableRenderDoc), CONFIG_RENDERDOC, CONFIG_RENDERDOC__ENABLED);
			_SettingsService->SetSetting(std::to_string(enableOverlay), CONFIG_RENDERDOC, CONFIG_RENDERDOC__DISPLAYOVERLAY);
			_SettingsService->SetSetting(std::to_string(autoOpenOnCapture), CONFIG_RENDERDOC, CONFIG_RENDERDOC__AUTOOPEN);
			_SettingsService->SetSetting(captureDir, CONFIG_RENDERDOC, CONFIG_RENDERDOC__CAPTUREDIR);

			ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Events::EventType::SettingsUpdated, std::make_shared<Events::SettingsUpdateEventArgs>());

		}
#else
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Renderdoc has not been build against this version of the Osiris Editor.");
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Please install RenderDoc 64-Bit and rebuild.");
#endif

	}
}