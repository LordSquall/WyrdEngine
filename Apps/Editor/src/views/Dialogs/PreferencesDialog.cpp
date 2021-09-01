#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "PreferencesDialog.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	PreferencesDialog::PreferencesDialog(EditorLayer* editorLayer) : EditorViewDialogBase("Preferences Viewer", editorLayer)
	{
		_SettingsService = ServiceManager::Get<SettingsService>();
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

		if (ImGui::Button("Scene Viewer", ImVec2(navigationPanelWidth, 0)))
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_SceneViewer, this);

		if (ImGui::Button("RenderDoc", ImVec2(navigationPanelWidth, 0)))
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_RenderDoc, this);

		if (ImGui::Button("Windows Player", ImVec2(navigationPanelWidth, 0)))
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_WindowsPlayer, this);

		if (ImGui::Button("External Tools", ImVec2(navigationPanelWidth, 0)))
			subMenu = std::bind(&PreferencesDialog::OnEditorRender_ExternalTools, this);

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
			ServiceManager::Get<EventService>()->Publish(Events::EventType::SettingsUpdated, std::make_unique<Events::SettingsUpdateEventArgs>());
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
#ifdef WYRD_RENDERDOC_ENABLED
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

			ServiceManager::Get<EventService>()->Publish(Events::EventType::SettingsUpdated, std::make_unique<Events::SettingsUpdateEventArgs>());

		}
#else
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Renderdoc has not been build against this version of the Wyrd Editor.");
		ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "Please install RenderDoc 64-Bit and rebuild.");
#endif

	}

	void PreferencesDialog::OnEditorRender_SceneViewer()
	{
		static bool enableGridWidget = Utils::ToBool(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ENABLED, std::string("0")));
		static int spacing[] = {
			Utils::ToInt(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__XSPACING, std::string("128"))),
			Utils::ToInt(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__YSPACING, std::string("128")))
		};
		static int rows = Utils::ToInt(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ROWS, std::string("12")));
		static int cols = Utils::ToInt(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLS, std::string("12")));
		static Color color = Utils::ToColor(_SettingsService->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLOR, std::string("0.2,0.2,0.2,1.0")));

		if (ImGui::CollapsingHeader("Grid Widget", ImGuiTreeNodeFlags_None))
		{
			ImGui::Checkbox("Enabled", &enableGridWidget);
			ImGui::InputInt2("Spacing", &spacing[0]);
			ImGui::InputInt("Row Count", &rows);
			ImGui::InputInt("Column Count", &cols);
			ImGui::ColorEdit4("Color", (float*)&color);
		}

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(std::to_string(enableGridWidget), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ENABLED);
			_SettingsService->SetSetting(std::to_string(spacing[0]), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__XSPACING);
			_SettingsService->SetSetting(std::to_string(spacing[1]), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__YSPACING);
			_SettingsService->SetSetting(std::to_string(rows), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__ROWS);
			_SettingsService->SetSetting(std::to_string(cols), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLS);
			_SettingsService->SetSetting(Utils::ToString(color), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__COLOR);

			ServiceManager::Get<EventService>()->Publish(Events::EventType::SettingsUpdated, std::make_unique<Events::SettingsUpdateEventArgs>());

		}
	}

	void PreferencesDialog::OnEditorRender_WindowsPlayer()
	{
		static std::string binPath = _SettingsService->GetSetting(CONFIG_WINDOWSPLAYER, CONFIG_WINDOWSPLAYER__BINPATH, std::string("../../bin/Debug/WindowsPlayer/WindowsPlayer.exe"));

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(binPath, CONFIG_WINDOWSPLAYER, CONFIG_WINDOWSPLAYER__BINPATH);

			ServiceManager::Get<EventService>()->Publish(Events::EventType::SettingsUpdated, std::make_unique<Events::SettingsUpdateEventArgs>());
		}
	}

	void PreferencesDialog::OnEditorRender_ExternalTools()
	{
		static std::string textureEditor = _SettingsService->GetSetting(CONFIG_EXTERNALTOOLS, CONFIG_EXTERNALTOOLS_TEXTUREEDITOR, std::string("my_captures/example"));

		ImGui::InputText("Texture Editor", &textureEditor);

		if (ImGui::Button("Apply") == true)
		{
			_SettingsService->SetSetting(textureEditor, CONFIG_EXTERNALTOOLS, CONFIG_EXTERNALTOOLS_TEXTUREEDITOR);

			ServiceManager::Get<EventService>()->Publish(Events::EventType::SettingsUpdated, std::make_unique<Events::SettingsUpdateEventArgs>());

		}
	}
}