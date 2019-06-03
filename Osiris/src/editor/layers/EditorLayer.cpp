#pragma once

#include "osrpch.h"
#include "EditorLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "editor/platform/OpenGL/imgui_opengl_renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#define GetCurrentDir _getcwd;

namespace Osiris::Editor
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };
	static ImFont*		s_defaultFont							= nullptr;
	std::string host_top_level_folder_prefix;
	std::filesystem::path p;
	

	EditorLayer::EditorLayer()
		: Layer("ImGuiLayer")
	{
		int status = gladLoadGL();
		OSR_CORE_ASSERT(status, "Failed to initialise GLAD!");

		m_plugins["Game Viewer"] = std::make_shared<GameViewer>();
		m_plugins["Scene Editor"] = std::make_shared<SceneEditor>();
		m_plugins["Scene Hierarchy"] = std::make_shared<SceneHierarchy>();
		m_plugins["Resource Viewer"] = std::make_shared<ResourceViewer>();
		m_plugins["Project Explorer"] = std::make_shared<ProjectExplorer>();

		host_top_level_folder_prefix = Get_TLF();
		p = Get_TLF();
		OSR_INFO("host_top_level_folder_prefix = " + host_top_level_folder_prefix);


	}

	EditorLayer::~EditorLayer()
	{

	}

	std::string EditorLayer::Get_TLF()
	{
		char top_level_folder_check[] = { 'O','s','i','r','i','s','E','n','g','i','n','e' };
		char top_level_folder[FILENAME_MAX] = {};
		char cCurrentPath[FILENAME_MAX];
		int bytes = GetModuleFileName(NULL, cCurrentPath, sizeof(cCurrentPath));
		unsigned j = 1;
		char sep = '\\';
		bool tlf = false;
		for (unsigned i = 0; i < bytes; i++) {
			if (cCurrentPath[i] == sep) {
				while (cCurrentPath[i + j] != sep) {
					if (cCurrentPath[i + j] == top_level_folder_check[j - 1]) {
						tlf = true;
					}
					else {
						tlf = false;
						break;
					}
					j++;
				};
				if (tlf) {
					const int tlf_length = i + j + 1;
					for (unsigned k = 0; k < i + j + 1; k++) {
						top_level_folder[k] = cCurrentPath[k];
					}
					break;
				}
				else {
					j = 1;
				}
			}

		}
		std::string tlf_path(top_level_folder);
		return tlf_path;
	}

	void EditorLayer::OnAttach()
	{
		/* load in icons sets */
		m_IconLibrary.AddIconsFromFile(std::string(host_top_level_folder_prefix + "Osiris/res/icons/filesystem_icons.json"));

		/* set the style and icons library for each of the plugins */
		for (std::map<std::string, std::shared_ptr<EditorPlugin>>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
		{
			(it->second)->SetIconLibrary(&m_IconLibrary);
			(it->second)->OnInitialise();
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendPlatformName = "imgui_openg3_renderer";

		if (std::filesystem::exists(host_top_level_folder_prefix + "Osiris/res/arial.ttf") == true)
		{
			s_defaultFont = io.Fonts->AddFontFromFileTTF((p + "Osiris/res/arial.ttf")), 16.0f);
			unsigned char * pixels;
			int width, height, bytes_per_pixels;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);
			unsigned int id = Application::Get().GetRenderer().LoadRawTexture(pixels, width, height, 4);
			io.Fonts->SetTexID((void*)id);
		}
		
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

	void EditorLayer::OnRender(std::shared_ptr<Renderer> renderer)
	{
		static bool menu_app_close_show = false;
		static bool menu_help_demo_window_show = false;


		float time = (float)glfwGetTime();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(s_defaultFont);

		/* menu bar */
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("Project"))
		{
			ImGui::MenuItem("Open");
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Save As..");
			ImGui::Separator();
			ImGui::MenuItem("Reload");
			ImGui::Separator();
			ImGui::MenuItem("Close"); 
			ImGui::Separator();
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
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}