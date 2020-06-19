#pragma once

#include "osrpch.h"

#include "PropertiesViewer.h"
#include "IPropertyComponent.h"

#include "core/Application.h"
#include "core/Layer.h"
#include "services/ServiceManager.h"

#include <imgui.h>

namespace Osiris::Editor
{
	std::shared_ptr<GameObject> PropertiesViewer::_SelectedGameObject = NULL;
	std::shared_ptr<Resource> PropertiesViewer::_SelectedAsset = NULL;

	PropertiesViewer::PropertiesViewer() : EditorViewBase("Properties"), _Mode(None)
	{
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(PropertiesViewer::OnSelectedGameObjectChanged));
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SelectedAssetChanged, EVENT_FUNC(PropertiesViewer::OnSelectedAssetChanged));
	}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		switch (_Mode)
		{
		case None:
			ImGui::Text("No Context Selected");
			break;
		case GameObjectUI:
			DrawGameObjectUI();
			break;
		case AssetUI:
			DrawAssetUI();
			break;
		}
	}

	void PropertiesViewer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;

		_Mode = GameObjectUI;
	}

	void PropertiesViewer::OnSelectedAssetChanged(Events::EventArgs& args)
	{
		Events::SelectedAssetChangedArgs& evtArgs = static_cast<Events::SelectedAssetChangedArgs&>(args);
		_SelectedAsset = evtArgs.resource;
		_Mode = AssetUI;
	}

	void PropertiesViewer::DrawGameObjectUI()
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

			_SelectedGameObject->transform2d.OnPropertyEditorDraw();			
			_SelectedGameObject->spriteRender.OnPropertyEditorDraw();
			_SelectedGameObject->script.OnPropertyEditorDraw();

		}
	}

	void PropertiesViewer::DrawAssetUI()
	{
		if (_SelectedAsset != NULL)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "No Assets");
		}
	}
}