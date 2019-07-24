#pragma once

#include "osrpch.h"

#include "PropertiesViewer.h"

#include "editor/services/ServiceManager.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	Sprite* PropertiesViewer::_SelectedSprite = NULL;

	PropertiesViewer::PropertiesViewer() : EditorPlugin("Properties"), _Mode(None)
	{
		ServiceManager::Get<EventService>(ServiceManager::Service::Events).Subscribe(Events::EventType::SelectedSpriteChanged, EVENT_FUNC(PropertiesViewer::OnSelectedSpriteChanged));
	}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		ImGui::Begin("Properties");

		switch (_Mode)
		{
		case None:
			break;
		case SpriteUI:
			DrawSpriteUI();
			break;
		}
		ImGui::End();

	}

	void PropertiesViewer::OnSelectedSpriteChanged(Events::EventArgs& args)
	{
		Events::SelectedSpriteChangedArgs& evtArgs = static_cast<Events::SelectedSpriteChangedArgs&>(args);

		_SelectedSprite = (Sprite*)evtArgs.sprite;

		_Mode = SpriteUI;
	}


	void PropertiesViewer::DrawSpriteUI()
	{
		if (_SelectedSprite != NULL)
		{

			static char spriteName[32] = "SpriteName";
			strcpy(spriteName, _SelectedSprite->GetName().c_str());
			ImGui::Text("Name:");
			ImGui::SameLine();
			if (ImGui::InputText("##edit", spriteName, IM_ARRAYSIZE(spriteName)) == true)
			{
				_SelectedSprite->SetName(std::string(spriteName));
			}

			ImGui::Separator();

			if (ImGui::TreeNode("Position"))
			{
				float x = (float)_SelectedSprite->GetX();
				float y = (float)_SelectedSprite->GetY();

				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::PushID("X");
				if (ImGui::InputFloat("", &x))
					_SelectedSprite->SetX((int)x);
				ImGui::PopID();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::PushID("Y");
				if (ImGui::InputFloat("", &y))
					_SelectedSprite->SetY((int)y);
				ImGui::PopID();

				ImGui::TreePop();
			}
		}
	}
}