#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/ecs/Components.h>
#include <core/ecs/ComponentPool.h>

/* local includes */
#include "PropertiesViewer.h"
#include "views/PropertiesViewer/ComponentViews/ComponentViewFactory.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	Wyrd::Entity PropertiesViewer::_SelectedEntity = 0;

	PropertiesViewer::PropertiesViewer(EditorLayer* editorLayer) : EditorViewBase("Properties", editorLayer)
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);

		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, EVENT_FUNC(PropertiesViewer::OnSelectedEntityChanged));
	}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		DrawEntityUI();
	}

	void PropertiesViewer::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

		_SelectedEntity = evtArgs.entity;
	}

	void PropertiesViewer::DrawEntityUI()
	{
		if (_SelectedEntity == ENTITY_INVALID)
		{
			return;
		}

		auto scene = _WorkspaceService->GetLoadedScene();

		/* Process each of the registered component views to see if the entity has a matching assigned component */
		std::bitset<64> mask = scene->GetMask(_SelectedEntity);

		for (int i = 0; i < scene->componentPools.size(); ++i)
		{
			if (mask.test(i))
			{
				bool removed = false;

				ImGui::PushID(i);
				ImGui::Text(scene->componentPools[i]->scriptName.c_str());

				/* we cant allow the metadata component to be deleted */
				if (i != 0)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("x"))
					{
						scene->RemoveComponent(i, _SelectedEntity);
						removed = true;
					}
				}

				if (removed == false)
					ComponentViewFactory::Create(scene->componentPools[i]->name, scene->Get(i, _SelectedEntity));
				
				ImGui::Separator();
				ImGui::PopID();
			}
		}
	}
}