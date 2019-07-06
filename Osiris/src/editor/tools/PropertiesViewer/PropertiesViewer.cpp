#pragma once

#include "osrpch.h"

#include "PropertiesViewer.h"

#include "editor/events/EventsManager.h"
#include "core/Application.h"
#include "core/Layer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	Sprite* PropertiesViewer::_SelectedSprite = NULL;

	PropertiesViewer::PropertiesViewer() : EditorPlugin("Properties"), _Mode(None)
	{
		Events::EventsManager::Subscribe(Events::EventType::SelectedSpriteChanged, EVENT_FUNC(PropertiesViewer::OnSelectedSpriteChanged));
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
				float x = _SelectedSprite->GetX();
				float y = _SelectedSprite->GetY();

				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::PushID("X");
				if (ImGui::InputFloat("", &x))
					_SelectedSprite->SetX(x);
				ImGui::PopID();

				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::PushID("Y");
				if (ImGui::InputFloat("", &y))
					_SelectedSprite->SetY(y);
				ImGui::PopID();

				ImGui::TreePop();
			}
		}
	}
}