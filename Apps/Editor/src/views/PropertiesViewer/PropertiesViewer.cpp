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
	PropertiesViewer::PropertiesViewer(EditorLayer* editorLayer) : EditorViewBase("Properties", editorLayer), _EventService(nullptr), _WorkspaceService(nullptr), _SelectedEntity(ENTITY_INVALID), _SelectedAsset(nullptr), _GUIFunc(nullptr)
	{
		_EventService = ServiceManager::Get<EventService>();
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();

		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(PropertiesViewer::OnSelectedEntityChanged));

		_EventService->Subscribe(Events::EventType::SelectedAssetChanged, WYRD_BIND_FN(PropertiesViewer::OnSelectedResourceChanged));
	}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		if (_GUIFunc)
			_GUIFunc();
	}

	void PropertiesViewer::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);

		_SelectedEntity = evtArgs.entity;

		_GUIFunc = std::bind(&PropertiesViewer::DrawEntityUI, this);
	}
	
	void PropertiesViewer::OnSelectedResourceChanged(Events::EventArgs& args)
	{
		Events::SelectedAssetChangedArgs& evtArgs = static_cast<Events::SelectedAssetChangedArgs&>(args);

		_SelectedAsset = evtArgs.resource;

		_GUIFunc = std::bind(&PropertiesViewer::DrawResourceUI, this);
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

				if (ComponentViewFactory::Contains(scene->componentPools[i]->name, _SelectedEntity))
				{
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
						ComponentViewFactory::Create(scene->componentPools[i]->name, _SelectedEntity, scene->Get(i, _SelectedEntity));

					ImGui::Separator();
					ImGui::PopID();
				}
			}
		}
	}

	void PropertiesViewer::DrawResourceUI()
	{
		ImGui::Text(_SelectedAsset->GetName().c_str());
	}
}