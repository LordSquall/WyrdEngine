#include "osrpch.h"

#include "ImGuiLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "platform/OpenGL/imgui_opengl_renderer.h"
#include <GLFW/glfw3.h>

namespace Osiris
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
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

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnRender(std::shared_ptr<Renderer> renderer)
	{
		static bool menu_app_close_show = true;
		static bool menu_tools_show = true;
		static bool menu_tools_resources_show = true;


		float time = (float)glfwGetTime();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		ImGui_ImplOpenGL3_NewFrame();
		UpdateMouse();
		UpdateCursor();
		ImGui::NewFrame();

		/* menu bar */
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("App", true))
		{
			if (ImGui::MenuItem("Exit", NULL, &menu_app_close_show))
			{

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools", true))
		{
			ImGui::MenuItem("Resources", NULL, &menu_tools_resources_show);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		if(menu_tools_resources_show == true) m_ResourceViewer.OnEditorRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
	}

	void ImGuiLayer::OnEvent(Event& event)
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

	void ImGuiLayer::UpdateMouse()
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

	void ImGuiLayer::UpdateCursor()
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