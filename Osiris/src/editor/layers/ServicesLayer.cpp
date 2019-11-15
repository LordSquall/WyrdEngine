#pragma once

#include "osrpch.h"
#include "ServicesLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "editor/services/ServiceManager.h"

#include "editor/tools/PropertiesViewer/PropertiesViewer.h"
#include "editor/tools/LayerViewer/LayerViewer.h"
#include "editor/tools/SpriteLayerEditor/SpriteLayerEditor.h"
#include "editor/tools/AssetViewer/AssetViewer.h"

#include "editor/platform/OpenGL/imgui_opengl_renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <shellapi.h>

namespace Osiris::Editor
{

	ServicesLayer::ServicesLayer()
		: Layer("ServicsLayer")
	{
	}

	ServicesLayer::~ServicesLayer()
	{

	}

	void ServicesLayer::OnAttach()
	{
	}

	void ServicesLayer::OnDetach()
	{

	}

	void ServicesLayer::OnRender(Timestep ts, Renderer& renderer)
	{
	}
}