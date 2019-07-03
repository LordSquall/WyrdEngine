#pragma once

#include "osrpch.h"

#include "PropertiesViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{
	PropertiesViewer::PropertiesViewer() : EditorPlugin("Properties") {}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		ImGui::Begin("Properties");

		ImGui::End();
	}

}