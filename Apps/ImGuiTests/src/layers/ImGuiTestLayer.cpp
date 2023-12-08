#include "ImGuiTestLayer.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/Maths.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/renderer/FontType.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <../imgui/imgui.h>
#include <../imgui/imgui_internal.h>
#include <../imgui/misc/cpp/imgui_stdlib.h>
#include <platform/imgui_opengl_renderer.h>

#include <ImGuiEx.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

static bool			s_MouseJustPressed[5] = { false, false, false, false, false };
static GLFWcursor*	s_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };
static ImFont*		s_defaultFont = nullptr;
static float		s_time = 0.0f;

static Scene		s_Scene;

void BuildSample()
{
	s_Scene.Initialise();

	s_Scene.CreateEntity("Entity_A");
	s_Scene.CreateEntity("Entity_B");
	s_Scene.CreateEntity("Entity_C");
	s_Scene.CreateEntity("Entity_D");
	s_Scene.CreateEntity("Entity_F");
	s_Scene.CreateEntity("Entity_G");
	s_Scene.CreateEntity("Entity_H");
}

bool ImGuiTestLayer::OnAttach()
{
	/* create the ImGui content info */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	/* retrieve the io and style subsystems from imgui*/
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	/* set the configuration flags for imgui */
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	/* add the editor gui font */
	//io.Fonts->AddFontFromFileTTF("Montserrat-Regular.otf", 16.0f);

	/* set the background flags for imgui flags */
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

	/* build the array of cursors to map to imgui */
	s_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	s_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	s_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
	s_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	s_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	s_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	s_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	s_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	/* Set style variables */
	style.WindowRounding = 0.0f;

	ImVec4* colors = style.Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style.ChildRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.FrameRounding = 2.0f;
	style.GrabMinSize = 7.0f;
	style.PopupRounding = 2.0f;
	style.ScrollbarRounding = 12.0f;
	style.ScrollbarSize = 13.0f;
	style.TabBorderSize = 1.0f;
	style.TabRounding = 0.0f;
	style.WindowRounding = 4.0f;

	/* initialise imgui */
	ImGui_ImplOpenGL3_Init("#version 410");

	BuildSample();

	return true;
}

void ImGuiTestLayer::OnDetach() { }

void ImGuiTestLayer::OnUpdate(Timestep ts) 
{
	
}

void ImGuiTestLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(ImGuiTestLayer::OnKeyReleasedEvent), nullptr);
	dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(ImGuiTestLayer::OnKeyPressedEvent), nullptr);
	dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(ImGuiTestLayer::OnMouseButtonPressedEvent), nullptr);
	dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(ImGuiTestLayer::OnMouseButtonReleasedEvent), nullptr);
	dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(ImGuiTestLayer::OnMouseMovedEvent), nullptr);
}

void ImGuiTestLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	/* retrieve the time to glfw */
	float time = (float)glfwGetTime();

	/* get references to the imgui io and Application objects */
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();

	/* update the io time and size fields */
	io.DeltaTime = s_time > 0.0 ? (time - s_time) : (1.0f / 60.0f);
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	/* store to the time */
	s_time = time;

	/* start a new imgui frame */
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	/* push the editor font */
	//ImGui::PushFont(s_defaultFont);

	/* create the main windows flags */
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	/* set the next window to match the viewport position and size */
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	//ImGui::SetNextWindowViewport(viewport->ID);

	/* setup the root window flags */
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	/* start root dockspace window */
	ImGui::Begin("DockspaceRoot", 0, window_flags);

	for (Entity e : EntitySet<RelationshipComponent, MetaDataComponent>(s_Scene))
	{
		MetaDataComponent* metatDataComponent = s_Scene.Get<MetaDataComponent>(e);
		RelationshipComponent* relationshipComponent = s_Scene.Get<RelationshipComponent>(e);

		if (relationshipComponent->parent == ENTITY_INVALID)
		{
			if (ImGui::TreeEntity(s_Scene, e))
			{

			}
		}
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiTestLayer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 2);
	return false;
}

bool ImGuiTestLayer::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
{
	if (e.GetRepeatCount() == 1)
	{
		Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 0);
	}
	else
	{
		Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 1);
	}
	return false;
}

bool ImGuiTestLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseButtonState(e.GetMouseButton(), true);
	return false;
}

bool ImGuiTestLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseButtonState(e.GetMouseButton(), false);
	return false;
}

bool ImGuiTestLayer::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseState(e.GetX(), e.GetY());
	return false;
}