#pragma once

#include "osrpch.h"
#include "EditorLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "editor/tools/PropertiesViewer/PropertiesViewer.h"
#include "editor/tools/LayerViewer/LayerViewer.h"
#include "editor/tools/SpriteLayerEditor/SpriteLayerEditor.h"

#include "editor/platform/OpenGL/imgui_opengl_renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

namespace Osiris::Editor
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };
	static ImFont*		s_defaultFont							= nullptr;
	Utils util;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		Application& app = Application::Get();
		LayerStack* stack = app.GetLayerStack();

		m_plugins["Properties"] = std::make_shared<PropertiesViewer>();

		std::shared_ptr<LayerViewer> layerViewer = std::make_shared<LayerViewer>();
		layerViewer->SetLayerStack(stack);

		m_plugins["Layer Viewer"] = layerViewer;

		std::shared_ptr<SpriteLayerEditor> spriteLayerEditor = std::make_shared<SpriteLayerEditor>();
		spriteLayerEditor->SetRenderingLayer((Layers::Renderer2DLayer*)stack->FindLayer("2D Rendering Layer"));
		m_plugins["Sprite Layer Editor"] = spriteLayerEditor;

		/* Register all editor plugins for events */
		//for(auto& )

		util = Utils();
	}

	EditorLayer::~EditorLayer()
	{

	}

	void EditorLayer::OnAttach()
	{
		/* load in icons sets */
		m_IconLibrary.AddIconsFromFile(util.GetAssetFolder() + /*std::string("../../Osiris/res*/"/icons/filesystem_icons.json");//TODO confirm with Paul this is the correct way to do this

		/* set the style and icons library for each of the plugins */
		for (std::map<std::string, std::shared_ptr<EditorPlugin>>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
		{
			(it->second)->SetIconLibrary(&m_IconLibrary);
			(it->second)->OnInitialise();
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendPlatformName = "imgui_openg3_renderer";
		
		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		s_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		s_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		s_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		s_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		s_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		s_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		s_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		s_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnRender(Renderer& renderer)
	{
		static bool menu_app_close_show = false;
		static bool menu_help_demo_window_show = false;


		float time = (float)glfwGetTime();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());


		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(s_defaultFont);

		/* menu bar */
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::BeginMenu("Create New Project...")) {
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {
					util.OpenFolderDialouge();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Exit"))
			{
				exit(0);
			}
			ImGui::EndMenu();
		}
		

		if (ImGui::BeginMenu("Tools", true))
		{
			for (std::map<std::string, std::shared_ptr<EditorPlugin>>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
			{
				ImGui::MenuItem((it->second)->GetName().c_str(), NULL, (it->second)->GetShowFlagRef());
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help", true))
		{
			ImGui::MenuItem("Show Demo Window", NULL, &menu_help_demo_window_show);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		/* system info window */
		ImGui::SetNextWindowSize(ImVec2((float)app.GetWindow().GetWidth() * 0.25f, (float)app.GetWindow().GetHeight() * 0.25f));
		ImGui::SetNextWindowPos(ImVec2(((float)app.GetWindow().GetWidth() * 0.75f) - 5.0f, 25.0f));
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav;

		ImGui::Begin("System Info", NULL, window_flags);

		struct RendererInfo& info = renderer.GetVendorInfo();
		ImGui::Text("Vendor: %s", info.vendor.c_str());
		ImGui::Text("Version: %s", info.version.c_str());
		ImGui::Text("Renderer: %s", info.renderer.c_str());

		ImGui::End();
		
		/* test the plugin visibility flags */
		for (std::map<std::string, std::shared_ptr<EditorPlugin>>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
		{
			bool showFlag = *(it->second)->GetShowFlagRef();
			if (showFlag == true)
			{
				(it->second)->OnEditorRender();
			}
		}

		if (menu_help_demo_window_show == true) ImGui::ShowDemoWindow();

		ImGui::PopFont();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(OSR_BIND_EVENT_FN(EditorLayer::OnWindowResizeEvent));
	}

	bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool EditorLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool EditorLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool EditorLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		return false;
	}

	bool EditorLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	bool EditorLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() > 0 && e.GetKeyCode() < 0x10000)
			io.AddInputCharacter((unsigned short)e.GetKeyCode());

		return false;
	}

	bool EditorLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}