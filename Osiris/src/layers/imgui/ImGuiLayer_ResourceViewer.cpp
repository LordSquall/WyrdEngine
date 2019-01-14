#include "osrpch.h"

#include "ImGuiLayer_ResourceViewer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "platform/OpenGL/imgui_opengl_renderer.h"
#include <GLFW/glfw3.h>

namespace Osiris
{

	ImGuiLayer_ResourceViewer::ImGuiLayer_ResourceViewer() : ImGuiPlugin("Resource Viewer")
	{

	}

	ImGuiLayer_ResourceViewer::~ImGuiLayer_ResourceViewer()
	{

	}

	void ImGuiLayer_ResourceViewer::OnEditorRender()
	{
		ImGui::Begin("Resource Viewer");

		ImGui::Text("Welcome to the Resource Viewer Plugin");

		ImGui::End();
	}

}