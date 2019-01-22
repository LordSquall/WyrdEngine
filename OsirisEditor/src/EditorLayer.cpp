#include "osreditorpch.h"

#include "EditorLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "platform/OpenGL/imgui_opengl_renderer.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>

namespace Osiris::Editor
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };
	static ImFont*		s_defaultFont							= nullptr;

	EditorLayer::EditorLayer()
		: Layer("ImGuiLayer")
	{
		//m_plugins["Game Viewer"] = std::make_shared<GameViewer>();
		//m_plugins["Scene Editor"] = std::make_shared<SceneEditor>();
		//m_plugins["Scene Hierarchy"] = std::make_shared<SceneHierarchy>();
		//m_plugins["Resource Viewer"] = std::make_shared<ResourceViewer>();
		//m_plugins["Project Explorer"] = std::make_shared<ProjectExplorer>();
	}

	EditorLayer::~EditorLayer()
	{

	}

	void EditorLayer::OnAttach()
	{

		/* load in icons sets */
		m_IconLibrary.AddIconsFromFile(std::string("C:/Projects/Active/OsirisEngine/Osiris/res/icons/filesystem_icons.json"));

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

		if (std::filesystem::exists("C:/Projects/Active/OsirisEngine/Osiris/res/arial.ttf") == true)
		{
			s_defaultFont = io.Fonts->AddFontFromFileTTF("C:/Projects/Active/OsirisEngine/Osiris/res/arial.ttf", 16.0f);
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
		UpdateMouse();
		UpdateCursor();
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
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("App", true))
		{
			if (ImGui::MenuItem("Exit", NULL, &menu_app_close_show))
			{

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
		int flags = event.GetCategoryFlags();
		unsigned int val = EventCategoryKeyboard;

		switch(event.GetCategoryFlags())
		{
		case EventCategoryKeyboard:
			{
				KeyEvent* keyEvent = (KeyEvent*)&event;

				ImGuiIO& io = ImGui::GetIO();
				if (keyEvent->GetEventType() == EventType::KeyPressed)
					io.KeysDown[keyEvent->GetKeyCode()] = true;

				if (keyEvent->GetEventType() == EventType::KeyReleased)
					io.KeysDown[keyEvent->GetKeyCode()] = false;

				// Modifiers are not reliable across systems
				io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
				io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
				io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
				io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
			}
			break;
		case EventCategoryMouse:
			{
				if (event.GetEventType() == EventType::MouseButtonPressed)
				{
					MouseButtonPressedEvent* mouseEvent = (MouseButtonPressedEvent*)&event;
					s_MouseJustPressed[mouseEvent->GetMouseButton()] = true;
				}
			}
			break;
		}
	}

	void EditorLayer::UpdateMouse()
	{
		Application& app = Application::Get();
		GLFWwindow* nativeWindow = (GLFWwindow*)app.GetWindow().GetNativeWindowPointer();
		ImGuiIO& io = ImGui::GetIO();

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{
			// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
			io.MouseDown[i] = s_MouseJustPressed[i] || glfwGetMouseButton(nativeWindow, i) != 0;
			s_MouseJustPressed[i] = false;
		}

		// Update mouse position
		const ImVec2 mouse_pos_backup = io.MousePos;
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
		const bool focused = glfwGetWindowAttrib(nativeWindow, GLFW_FOCUSED) != 0;
		if (focused)
		{
			if (io.WantSetMousePos)
			{
				glfwSetCursorPos(nativeWindow, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
			}
			else
			{
				double mouse_x, mouse_y;
				glfwGetCursorPos(nativeWindow, &mouse_x, &mouse_y);
				io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
			}
		}
	}

	void EditorLayer::UpdateCursor()
	{
		Application& app = Application::Get();
		GLFWwindow* nativeWindow = (GLFWwindow*)app.GetWindow().GetNativeWindowPointer();
		ImGuiIO& io = ImGui::GetIO();
		
		if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(nativeWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			return;

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
		{
			// Show OS mouse cursor
			// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
			glfwSetCursor(nativeWindow, s_MouseCursors[imgui_cursor] ? s_MouseCursors[imgui_cursor] : s_MouseCursors[ImGuiMouseCursor_Arrow]);
			glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}