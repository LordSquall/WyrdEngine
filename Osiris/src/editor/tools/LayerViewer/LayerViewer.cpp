#pragma once

#include "osrpch.h"

#include "LayerViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{
	bool v;

	LayerViewer::LayerViewer() : EditorPlugin("Layer Viewer") {}

	LayerViewer::~LayerViewer() {}

	void LayerViewer::OnEditorRender()
	{
		ImGui::Begin("Application Layer Viewer");

		for (auto layer : *_LayerStack)
		{
			/* ignore the editor layer */
			if (layer->GetName().compare("EditorLayer") != 0)
			{
				bool enabledFlag = layer->Enabled();
				ImGui::Checkbox(layer->GetName().c_str(), &enabledFlag);
				layer->Enabled(enabledFlag);
			}
		}

		ImGui::End();
	}

}