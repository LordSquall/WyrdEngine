#pragma once

#include "osrpch.h"

#include "SceneEditor.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "imgui.h"

namespace Osiris::Editor
{

	SceneEditor::SceneEditor() : EditorPlugin("Scene Editor")
	{
		
	}

	SceneEditor::~SceneEditor(){}

	void SceneEditor::OnEditorRender()
	{
		
	}

}