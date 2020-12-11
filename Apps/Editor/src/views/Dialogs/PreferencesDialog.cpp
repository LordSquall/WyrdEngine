#pragma once

#include "osrpch.h"

#include "PreferencesDialog.h"

#include "imgui.h"

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
		static bool enableRenderDoc = Utils::ToBool(_SettingsService->GetSetting(std::string("Preferences-RenderDoc"), std::string("enabled"), std::string("0")));
		static bool enableOverlay = Utils::ToBool(_SettingsService->GetSetting(std::string("Preferences-RenderDoc"), std::string("display_overlay"), std::string("0")));
		static int currentKey;
		ImGui::Checkbox("Enable Renderdoc", &enableRenderDoc);
		ImGui::Checkbox("Enable Overlay", &enableOverlay);
		ImGui::Text("Current Render Capture Key: F4");

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(std::to_string(enableRenderDoc), "Preferences-RenderDoc", "enabled");

			ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Events::EventType::SettingsUpdated, std::make_shared<Events::SettingsUpdateEventArgs>());

		}
#else
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Renderdoc has not been build against this version of the Osiris Editor.");
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Please install RenderDoc 64-Bit and rebuild.");
#endif

	}
}