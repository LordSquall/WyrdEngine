/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/KeyCodes.h>
#include <core/Application.h>
#include <core/Layer.h>

/* local includes */
#include "FileViewer.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "views/Dialogs/NewScriptDialog.h"
#include "views/Dialogs/NewSceneDialog.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	FileViewer::FileViewer(EditorLayer* editorLayer) : EditorViewBase("File Viewer", editorLayer)
	{
		/* cache the service(s) */
		_resourcesService = ServiceManager::Get<ResourceService>();
		_workspaceService = ServiceManager::Get<WorkspaceService>();
		_EventService = ServiceManager::Get<EventService>();
		_dialogService = ServiceManager::Get<DialogService>();
	}

	void FileViewer::OnEditorRender()
	{
		ImGui::Text("FileViewer");
	}
}