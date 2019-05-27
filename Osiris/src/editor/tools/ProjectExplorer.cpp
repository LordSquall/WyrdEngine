#pragma once

#include "osrpch.h"

#include "ProjectExplorer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{
	static Icon* sFolderIcon;
	static Icon* sFileIcon;

	unsigned int ProjectExplorer_entry::s_NextId = 0;

	void ProjectExplorer_data::Update(ProjectExplorer_entry& root)
	{
		for (const auto & entry : std::filesystem::directory_iterator(root.path))
		{
			if (entry.is_directory())
			{
				ProjectExplorer_entry newEntry = ProjectExplorer_entry(entry.path().string());
				Update(newEntry);
				root.children.insert(std::pair<std::string, ProjectExplorer_entry>(entry.path().filename().string(), newEntry));
			}
		}
	}

	ProjectExplorer::ProjectExplorer() : EditorPlugin("Project Explorer")
	{
		std::string root = std::string("./res/");

		ProjectExplorer_entry rootPath = ProjectExplorer_entry(root);

		m_FSData.Update(rootPath);

		m_FSData.filesystem.insert(std::pair<std::string, ProjectExplorer_entry>(root, rootPath));

	}

	ProjectExplorer::~ProjectExplorer(){}

	void ProjectExplorer::OnInitialise()
	{
		sFolderIcon = &m_IconLibrary->GetIcon(std::string("folder_open"));
		sFileIcon = &m_IconLibrary->GetIcon(std::string("unknown_file"));
	}

	void ProjectExplorer::OnEditorRender()
	{
		const static float panelSplitWidthMin = 200.0f;
		static float panelSplitWidth;
		static bool show;

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_MenuBar;
		
		/* start a new window */
		if(ImGui::Begin("Project Explorer", &m_ShowFlag, windowFlags));
		{
			/* limit the split width between the navigation and content panels */
			panelSplitWidth = ImGui::GetWindowContentRegionWidth() * 0.5f;
			if (panelSplitWidth > panelSplitWidthMin) panelSplitWidth = panelSplitWidthMin;

			/* start navigation panel */
			//ImGui::BeginChild("navigation_panel", ImVec2(panelSplitWidth, 0.0f), true, 0);
			//	PopulateNavigationPanel();
			//ImGui::EndChild();

			//ImGui::SameLine();

			///* start content panel */
			//ImGui::BeginChild("data_panel");
			//	PopulateContentPanel();
			//ImGui::EndChild();

			ImGui::End();
		}
	}


	void ProjectExplorer::PopulateNavigationPanel()
	{
		for (std::map<std::string, ProjectExplorer_entry>::iterator it = m_FSData.filesystem.begin(); it != m_FSData.filesystem.end(); ++it)
		{
			ProjectExplorer_entry entryData = it->second;
			
			ImGuiTreeNodeFlags flags = (entryData.children.size() == 0) ? ImGuiTreeNodeFlags_Bullet : 0;

			bool open = ImGui::TreeNodeEx((void*)&entryData.uid, flags, "%s", it->first.c_str());

			if (ImGui::IsItemClicked())
				m_CurrentDir = entryData.path;

			if (ImGui::IsItemClicked(1))
			{
				CreateFolder(entryData.path + "New Folder");
			}

			if (open)
			{
				PopulateSubFileTree(entryData);
				ImGui::TreePop();
			}
		}
	}

	void ProjectExplorer::PopulateContentPanel()
	{
		static ImVec2 itemSize = ImVec2(50.0f, 70.0f);
		static ImVec2 iconSize = ImVec2(50.0f, 50.0f);
		static ImVec2 tileSize = ImVec2(70.0f, 70.0f);

		ImGui::Text(">> %s", m_CurrentDir.c_str());

		/* count the number of items in the directory */
		unsigned int itemCnt = 0;
		for (const auto & entry : std::filesystem::directory_iterator(m_CurrentDir))
		{
			itemCnt++;
		}

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 totalWidth = ImGui::GetWindowContentRegionMax();
		int columnCnt = (int)totalWidth.x / (int)tileSize.x;

		ImGui::Columns(columnCnt, "mycolumns", false);

		for (const auto & entry : std::filesystem::directory_iterator(m_CurrentDir))
		{
			Icon icon;

			if (entry.is_directory())
			{
				icon = *sFolderIcon;
			}
			else
			{
				icon = m_IconLibrary->GetIconFromFileExtension(entry.path());
			}

			ImGui::BeginGroup();
			ImGui::PushID(entry.path().c_str());
			ImGui::Image((void*)icon.GetHandle(), iconSize, ImVec2(icon.GetUV0()[0], icon.GetUV0()[1]), ImVec2(icon.GetUV1()[0], icon.GetUV1()[1]));
			ImGui::Text(entry.path().filename().string().c_str());
			ImGui::PopID();
			ImGui::EndGroup();
			
			/* tooltip */
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(entry.path().string().c_str());
				ImGui::EndTooltip();
			}
			
			/* context menu (right-click) */
			if (ImGui::BeginPopupContextItem("item context menu"))
			{
				ImGui::Selectable("Open");
				ImGui::EndPopup();
			}

			ImGui::NextColumn();
		}
	}


	void ProjectExplorer::PopulateSubFileTree(ProjectExplorer_entry& entry)
	{
		for (std::map<std::string, ProjectExplorer_entry>::iterator it = entry.children.begin(); it != entry.children.end(); ++it)
		{
			ProjectExplorer_entry entryData = it->second;
			
			ImGuiTreeNodeFlags flags =
				(entryData.children.size() == 0) ? ImGuiTreeNodeFlags_Bullet : 0;

			bool open = ImGui::TreeNodeEx((void*)&entryData.uid, flags, "%s - %d",it->first.c_str(), entryData.uid);

			if (ImGui::IsItemClicked())
				m_CurrentDir = entryData.path;

			if (open)
			{
				PopulateSubFileTree(entryData);
				ImGui::TreePop();
			}
		}
	}


	void ProjectExplorer::CreateFolder(std::string& path)
	{
		std::string root = std::string("./res/");

		ProjectExplorer_entry rootPath = ProjectExplorer_entry(root);

		std::filesystem::create_directory(path);
		m_FSData.Update(rootPath);
	}
}