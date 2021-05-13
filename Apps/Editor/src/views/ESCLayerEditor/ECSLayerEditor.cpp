#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/scene/Layer2D.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/SceneComponentFactory.h>
#include <core/KeyCodes.h>
#include <core/ecs/ComponentSet.h>

/* local includes */
#include "ECSLayerEditor.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	ECSLayerEditor::ECSLayerEditor(EditorLayer* editorLayer) : EditorViewBase("ECS Layer Editor", editorLayer) { }

	ECSLayerEditor::~ECSLayerEditor() { }

	void ECSLayerEditor::OnInitialise()
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);

		//_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(ECSLayerEditor::OnSceneOpened));
	}

	void ECSLayerEditor::OnEditorRender()
	{
		static char layerName[64] = "LayerName";
		
		if (_WorkspaceService->IsSceneLoaded())
		{
			Scene& scene = *_WorkspaceService->GetLoadedScene();


			// build a list of layers
			std::set<std::string> layers;
			for (Entity e : ComponentSet<MetaDataComponent>(scene))
			{
				MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(e);

				layers.insert(metadataComp->layerName);
			}

			if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::BeginChild("_LayerStackViewChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .25f));
				if (ImGui::BeginTable("spriteLayerTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Actions");
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableHeadersRow();

					int rowIndexCnt = 0;

					for (auto& l : layers)
					{
						
						ImGui::PushID(&l);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
						ImGui::TableNextColumn();

						bool selected = false;
						if (ImGui::Selectable(l.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
						{

						}

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Rename"))
							{
								//_EditLayerNameMode = true;
							}
							ImGui::EndPopup();
						}
					
						ImGui::TableNextColumn();

						ImGui::PopID();

						rowIndexCnt++;
					}

					ImGui::EndTable();
				}

				ImGui::EndChild();
			}
		
			if (ImGui::CollapsingHeader("Layer Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::BeginChild("_LayerStackViewChild2", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowSize().y * .25f));
				if (ImGui::BeginTable("spriteLayerTable2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Actions");
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableHeadersRow();

					int rowIndexCnt = 0;

					for (Entity e : ComponentSet<MetaDataComponent>(scene))
					{
						MetaDataComponent* metadataComp = scene.Get<MetaDataComponent>(e);

						ImGui::PushID(&e);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
						ImGui::TableNextColumn();

						bool selected = false;
						if (ImGui::Selectable(metadataComp->name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
						{

						}

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Rename"))
							{
								//_EditLayerNameMode = true;
							}
							ImGui::EndPopup();
						}

						ImGui::TableNextColumn();

						ImGui::PopID();

						rowIndexCnt++;
					}

					ImGui::EndTable();
				}

				ImGui::EndChild();
			}
		}
	}
}