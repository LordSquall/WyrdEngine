#pragma once

#include "osrpch.h"

#include "ResourceViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{

	ResourceViewer::ResourceViewer() : EditorPlugin("Resource Viewer"){}

	ResourceViewer::~ResourceViewer(){}

	void ResourceViewer::OnEditorRender()
	{
		ImGui::Begin("Resource Viewer");
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("ResourcesTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Textures"))
			{
				ShowTextureDialog();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Shaders"))
			{
				ShowShaderDialog();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Materials"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	void ResourceViewer::ShowTextureDialog()
	{
		
	}

	void ResourceViewer::ShowShaderDialog()
	{
		

	}
}