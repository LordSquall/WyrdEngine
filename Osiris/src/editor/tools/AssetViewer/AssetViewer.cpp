#pragma once

#include "osrpch.h"

#include "AssetViewer.h"

#include "editor/services/ServiceManager.h"
#include "core/Application.h"
#include "core/Layer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	AssetViewer::AssetViewer() : EditorPlugin("Asset Viewer") {}

	AssetViewer::~AssetViewer() {}

	void AssetViewer::OnEditorRender()
	{
		ImGui::Begin("Asset Viewer");

		ImGui::End();
	}

}