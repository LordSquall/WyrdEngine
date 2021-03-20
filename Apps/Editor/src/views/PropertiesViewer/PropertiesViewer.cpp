#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Application.h>
#include <core/Layer.h>

/* local includes */
#include "PropertiesViewer.h"
#include "PropertyViewFactory.h"
#include "views/DataModels/components/Transform2DComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	Osiris::GameObject* PropertiesViewer::_SelectedGameObject = nullptr;
	std::shared_ptr<Resource> PropertiesViewer::_SelectedAsset = NULL;

	PropertiesViewer::PropertiesViewer(EditorLayer* editorLayer) : EditorViewBase("Properties", editorLayer), _Mode(None)
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);

		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(PropertiesViewer::OnSelectedGameObjectChanged));
		_EventService->Subscribe(Events::EventType::SelectedAssetChanged, EVENT_FUNC(PropertiesViewer::OnSelectedAssetChanged));
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
			for (auto& component : _SelectedGameObject->components)
			{
				component->debugOverlayFunction = nullptr;
			}
		}

		_SelectedGameObject = evtArgs.gameObject;

		RefreshComponentViews();
	}

	void PropertiesViewer::OnSelectedAssetChanged(Events::EventArgs& args)
	{
		Events::SelectedAssetChangedArgs& evtArgs = static_cast<Events::SelectedAssetChangedArgs&>(args);
		_SelectedAsset = evtArgs.resource;
		_Mode = AssetUI;
	}

	void PropertiesViewer::RefreshComponentViews()
	{
		/* clear the property views */
		_PropertiesViews.clear();

		/* we only need to create new views if we have a selected game object */
		if (_SelectedGameObject != nullptr)
		{
			/* create the required views, a transform view is always needed */
			if (_SelectedGameObject->transform->GetType() == SceneComponentType::Transform2D)
			{
				_PropertiesViews.push_back(std::move(PropertyViewFactory::Create(_SelectedGameObject->transform.get(), _SelectedGameObject)));
			}

			for (auto& component : _SelectedGameObject->components)
			{
				_PropertiesViews.push_back(std::move(PropertyViewFactory::Create(component.get(), _SelectedGameObject)));
			}

			_Mode = GameObjectUI;
		}
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

			
			int componentIdx = 0;
			int removableIdx = -1;
			for (auto& view : _PropertiesViews)
			{
				ImGui::AlignTextToFramePadding();
				if (ImGui::TreeNodeEx(view->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap))
				{
					if (view->GetEnabledControls())
					{
						ImGui::SameLine(ImGui::GetWindowWidth() - 30);
						if (ImGui::Button("x"))
						{
							removableIdx = componentIdx - 1;
						}
					}

					view->OnPropertyEditorDraw();

					ImGui::TreePop();
				}
				componentIdx++;
			}

			if (removableIdx != -1)
			{
				_SelectedGameObject->components.erase(_SelectedGameObject->components.begin() + removableIdx);

				RefreshComponentViews();

				removableIdx = -1;
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