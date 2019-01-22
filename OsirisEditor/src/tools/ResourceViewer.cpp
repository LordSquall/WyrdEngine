#include "osreditorpch.h"

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
		static std::shared_ptr<Osiris::Texture>	selectedTexture = nullptr;
		float width = ImGui::GetWindowContentRegionWidth() * 0.5f;

		if (width > 200.0f)
		{
			width = 200.0f;
		}

		ImGui::BeginChild("selection_panel", ImVec2(width, 0.0f), true, 0);
		for (std::vector<std::shared_ptr<Osiris::Texture>>::iterator it = Resources::Get().m_TextureMap.begin(); it != Resources::Get().m_TextureMap.end(); ++it)
		{
			if (ImGui::Selectable((*it)->GetName().c_str()))
			{
				selectedTexture = *it;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (selectedTexture != nullptr)
		{
			ImVec2 imgSize(selectedTexture->GetWidth(), selectedTexture->GetHeight());
			ImGui::BeginChild("data_panel");
			ImGui::Image((void*)selectedTexture->GetHandle(), imgSize);
			ImGui::Text("Name: %s", selectedTexture->GetName().c_str());
			ImGui::Text("Source File: %s", selectedTexture->GetSourceFileName().c_str());
			ImGui::EndChild();
		}
	}

	void ResourceViewer::ShowShaderDialog()
	{
		static std::shared_ptr<Osiris::Shader>	selectedShader = nullptr;
		float width = ImGui::GetWindowContentRegionWidth() * 0.5f;

		if (width > 200.0f)
		{
			width = 200.0f;
		}

		ImGui::BeginChild("selection_panel", ImVec2(width, 0.0f), true, 0);
		for (std::vector<std::shared_ptr<Osiris::Shader>>::iterator it = Resources::Get().m_ShaderMap.begin(); it != Resources::Get().m_ShaderMap.end(); ++it)
		{
			if (ImGui::Selectable((*it)->GetName().c_str()))
			{
				selectedShader = *it;
			}
		}

		ImGui::EndChild();

		ImGui::SameLine();

		if (selectedShader != nullptr)
		{
			ImGui::BeginChild("data_panel");
			ImGui::Text("%s", selectedShader->GetVertexSource().c_str());
			ImGui::Text("%s", selectedShader->GetFragmentSource().c_str());
			ImGui::EndChild();
		}

	}
}