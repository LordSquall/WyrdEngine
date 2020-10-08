#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Application.h>
#include <core/Layer.h>

/* local includes */
#include "PropertiesViewer.h"
#include "PropertyViewFactory.h"
#include "views/DataModels/components/Transform2DView.h"
#include "services/ServiceManager.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	std::shared_ptr<Osiris::GameObject> PropertiesViewer::_SelectedGameObject = NULL;
	std::shared_ptr<Resource> PropertiesViewer::_SelectedAsset = NULL;

	PropertiesViewer::PropertiesViewer(EditorLayer* editorLayer) : EditorViewBase("Properties", editorLayer), _Mode(None)
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

		/* before changes the selected game object, we need to clear any component state values such as debug overlays etc.... */
		if (_SelectedGameObject != nullptr)
		{
			for (auto component : _SelectedGameObject->components)
			{
				component->debugOverlayFunction = nullptr;
			}
		}

		_SelectedGameObject = evtArgs.gameObject;

		/* clear the property views */
		_PropertiesViews.clear();

		/* we only need to create new views if we have a selected game object */
		if (_SelectedGameObject != nullptr)
		{
			/* create the required views */
			_PropertiesViews.push_back(std::make_shared<Transform2DView>(&*_SelectedGameObject->transform2D));

			for (auto component : _SelectedGameObject->components)
			{
				std::shared_ptr<IPropertiesView> newView = PropertyViewFactory::Create(component, &*_SelectedGameObject);
				_PropertiesViews.push_back(newView);
				component->debugOverlayFunction = std::bind(&IPropertiesView::OnSceneViewerDraw, newView, std::placeholders::_1);
			}

			_Mode = GameObjectUI;
		}
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


			for (auto& view : _PropertiesViews)
			{
				view->OnPropertyEditorDraw();
			}
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