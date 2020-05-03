#pragma once

#include "osrpch.h"

#include "PropertiesViewer.h"
#include "IPropertyComponent.h"

#include "services/ServiceManager.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	std::shared_ptr<GameObject> PropertiesViewer::_SelectedGameObject = NULL;

	PropertiesViewer::PropertiesViewer() : EditorPlugin("Properties"), _Mode(None)
	{
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(PropertiesViewer::OnSelectedGameObjectChanged));
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

	void PropertiesViewer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;

		_Mode = SpriteUI;
	}


	void PropertiesViewer::DrawSpriteUI()
	{
		if (_SelectedGameObject != NULL)
		{

			static char spriteName[32] = "SpriteName";
			strcpy(spriteName, _SelectedGameObject->name.c_str());
			ImGui::Text("Name:");
			ImGui::SameLine();
			if (ImGui::InputText("##edit", spriteName, IM_ARRAYSIZE(spriteName)) == true)
			{
				_SelectedGameObject->name = std::string(spriteName);
			}

			ImGui::Separator();

			_SelectedGameObject->transform2d->OnPropertyEditorDraw();
			_SelectedGameObject->spriteRender->OnPropertyEditorDraw();
		}
	}
}