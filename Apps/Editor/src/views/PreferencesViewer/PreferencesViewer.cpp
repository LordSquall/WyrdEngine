#pragma once

#include "osrpch.h"

#include "PreferencesViewer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	PreferencesViewer::PreferencesViewer(EditorLayer* editorLayer) : EditorViewBase("Preferences Viewer", editorLayer)
	{
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Service::Settings);
	}

	PreferencesViewer::~PreferencesViewer() {}

	void PreferencesViewer::OnEditorRender()
	{
		float navigationPanelWidth = 100.0f;
		static unsigned int menuID = 0;

		ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y));
		
		if (ImGui::Button("RenderDoc", ImVec2(navigationPanelWidth, 0)) == true)
		{
			menuID = 0;
		}

		ImGui::EndChildFrame();

		ImGui::SameLine();
		ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
		
		switch (menuID)
		{
		case 0: OnEditorRender_RenderDoc(); break;
		default:
			break;
		}

		ImGui::EndChildFrame();
	}


	void PreferencesViewer::OnEditorRender_RenderDoc()
	{
#ifdef OSR_RENDERDOC_ENABLED
		static bool enableRenderDoc = Utils::ToBool(_SettingsService->GetSetting(std::string("Preferences-RenderDoc"), std::string("enabled"), std::string("0")));
		static bool enableOverlay = Utils::ToBool(_SettingsService->GetSetting(std::string("Preferences-RenderDoc"), std::string("display_overlay"), std::string("0")));
		static int currentKey;
		ImGui::Checkbox("Enable Renderdoc", &enableRenderDoc);
		ImGui::Checkbox("Enable Overlay", &enableOverlay);
		ImGui::Text("Current Render Capture Key: F4");
		if (ImGui::Button("Cancel") == true)
		{
			Close();
		}
		ImGui::SameLine();

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(std::to_string(enableRenderDoc), "Preferences-RenderDoc", "enabled");

			ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Events::EventType::SettingsUpdated, std::make_shared<Events::SettingsUpdateEventArgs>());

			Close();
		}
#else
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Renderdoc has not been build against this version of the Osiris Editor.");
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Please install RenderDoc 64-Bit and rebuild.");
#endif

	}
}