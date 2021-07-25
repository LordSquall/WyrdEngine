#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/KeyCodes.h>
#include <core/Application.h>
#include <core/Layer.h>

/* local includes */
#include "ResourceViewer.h"
#include "datamodels/resources/Resource.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	ResourceViewer::ResourceViewer(EditorLayer* editorLayer) : EditorViewBase("Resource Viewer", editorLayer)
	{
		/* cache the service(s) */
		_resourcesService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_dialogService = ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog);
	}

	void ResourceViewer::OnEditorRender()
	{
		ImGui::Text("ResourceViewer");
	}
}