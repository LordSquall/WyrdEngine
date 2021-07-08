#pragma once

/* local includes */
#include "EditorLayer.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"
#include "services/ServiceManager.h"
#include "services/SimulationService.h"
#include "views/SceneViewer/SceneViewer.h"
#include "views/GameViewer/GameViewer.h"
#include "views/PropertiesViewer/PropertiesViewer.h"
#include "views/LayerViewer/LayerViewer.h"
#include "views/AssetViewer/AssetViewer.h"
#include "views/OutputView/OutputView.h"
#include "views/ExportView/ExportView.h"
#include "views/SceneEditor/SceneEditor.h"
#include "views/Dialogs/PreferencesDialog.h"
#include "views/Dialogs/NewProjectDialog.h"
#include "platform/OpenGL/imgui_opengl_renderer.h"
#include "support/ImGuiUtils.h"

/* core includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/maths/Rect.h>

/* external includes */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <shellapi.h>

namespace Wyrd::Editor
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

		_views["Scene Viewer"] = std::make_shared<SceneViewer>(this);
		_views["Game Viewer"] = std::make_shared<GameViewer>(this);
		_views["Properties"] = std::make_shared<PropertiesViewer>(this);
		_views["Scene Editor"] = std::make_shared<SceneEditor>(this);
		_views["Asset Viewer"] = std::make_shared<AssetViewer>(this);
		_views["Output"] = std::make_shared<OutputView>(this);
		_views["Export View"] = std::make_shared<ExportView>(this);

		util = Utils();

		/* cache services */
		_eventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_settingsService = ServiceManager::Get<SettingsService>(ServiceManager::Service::Settings);
		_resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);
		_simulationService = ServiceManager::Get<SimulationService>(ServiceManager::Service::Simulation);
		_dialogService = ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog);

		/* pass the scene viewer view to the simluation service */
		_simulationService->SetSceneViewer(_views["Scene Viewer"]);

		/* cache icon resources */
		_playButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_play");
		_stopButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_stop");

		/* setup event bindings */
		_eventService->Subscribe(Editor::Events::EventType::SceneOpened, EVENT_FUNC(EditorLayer::OnSceneOpened));
	}

	EditorLayer::~EditorLayer()
	{

	}

	bool EditorLayer::OnAttach()
	{
		/* load in icons sets */
		_resourceService->GetIconLibrary().AddIconsFromFile(std::string("res/icons/filesystem_icons.json"));

		/* set the style and icons library for each of the plugins */
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			(it->second)->OnInitialise();
		}

		/* set the starting tile field */
		_time = 0.0f;

		/* create the ImGui content info */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		/* retrieve the io and style subsystems from imgui*/
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		/* set the configuration flags for imgui */
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		/* add the editor gui font */
		io.Fonts->AddFontFromFileTTF("Montserrat-Regular.otf", 16.0f);

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

		/* attempt to load the default project otherwise asked the user to create a new one */
		if (_workspaceService->LoadProject(_settingsService->GetSetting(CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT, "")) == false)
		{
			_dialogService->OpenDialog(std::make_shared<NewProjectDialog>(this));
		}

		return true;
	}

	void EditorLayer::OnDetach()
	{
		ImGui::SaveIniSettingsToDisk("imgui.ini");
	}
	
	void EditorLayer::OnUpdate(Timestep ts)
	{
		_resourceService->OnUpdate();
		_eventService->OnUpdate();
		_simulationService->OnUpdate();
		_simulationService->Update(ts);
	}

	void EditorLayer::OnRender(Timestep ts, Renderer& renderer)
	{
		/* retrieve the time to glfw */
		float time = (float)glfwGetTime();

		/* get references to the imgui io and Application objects */
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		/* update the io time and size fields */
		io.DeltaTime = _time > 0.0 ? (time - _time) : (1.0f / 60.0f);
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		/* store to the time */
		_time = time;

		/* start a new imgui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		/* push the editor font */
		ImGui::PushFont(s_defaultFont);

		/* create the main windows flags */
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
		/* set the next window to match the viewport position and size */
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		/* setup the root window flags */
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		/* start root dockspace window */
		ImGui::Begin("DockspaceRoot", 0, window_flags);

		/* menu menu start */
		ImGui::BeginMainMenuBar();

		static bool menu_help_demo_window_show = false;

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::BeginMenu("Create New Project...")) {
				if (ImGui::MenuItem("New")) 
				{
					_dialogService->OpenDialog(std::make_shared<NewProjectDialog>(this));
				}
				if (ImGui::MenuItem("Open File", "Ctrl+O")) {
					WYRD_INFO(util.OpenFileDialog("*.scene"));
				}
				if (ImGui::MenuItem("Open Folder")) {
					WYRD_INFO(util.OpenFolderDialog());
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open Project"))
			{
				_workspaceService->LoadProject(util.OpenFileDialog(".oproj"));
			}
			if (ImGui::MenuItem("Open Project In Explorer", nullptr, nullptr, _workspaceService->IsProjectLoaded()))
			{
				//ShellExecuteA(NULL, "open", Utils::GetAssetFolder().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			ImGui::Separator();

			bool fileOperationsFlag = _workspaceService->IsSceneLoaded() && _workspaceService->IsProjectLoaded();

			if (ImGui::MenuItem("New Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded())) {
				std::optional<std::string> filepath = util.SaveFile("*.scene");
				if (filepath.has_value())
				{
					_workspaceService->SaveSceneAs(filepath.value());
					_workspaceService->LoadScene(filepath.value());
				}
			}

			if (ImGui::MenuItem("Save Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded() && _workspaceService->IsSceneLoaded())) {
				if (_workspaceService->SaveScene() == true)
				{
					WYRD_CORE_INFO("Saved Scene");
				}
			}

			if (ImGui::MenuItem("Save Scene As..", nullptr, nullptr, _workspaceService->IsProjectLoaded() && _workspaceService->IsSceneLoaded())) {
				std::string filepath = util.SaveFileDialog("Scene Json Files", "scene");
				if (_workspaceService->SaveSceneAs(filepath) == true)
				{
					WYRD_CORE_INFO("Saved Scene As");
				}
			}

			if (ImGui::MenuItem("Open Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded())) {
				std::string filepath = util.OpenFileDialog(".scene");
				if (_workspaceService->LoadScene(filepath) == true)
				{
					WYRD_CORE_INFO("Opened Scene");
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				WindowCloseEvent e;
				app.OnEvent(e);
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Views", true))
		{
			for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
			{
				ImGui::MenuItem((it->second)->GetName().c_str(), NULL, (it->second)->GetShowFlagRef());
			}


			ImGui::Separator();

			if (ImGui::BeginMenu("Layout"))
			{
				if (ImGui::MenuItem("Load..."))
				{
					std::string file = util.OpenFileDialog(".ini");
					ImGui::LoadIniSettingsFromDisk(file.c_str());
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Preferences"))
			{
				_dialogService->OpenDialog(std::make_shared<PreferencesDialog>(this));
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help", true))
		{
			ImGui::MenuItem("Show Demo Window", NULL, &menu_help_demo_window_show);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 64.0f) * 0.5f);

		/* primary control bar */
		if (ImGui::IconButton(_playButtonIcon, 1, !_simulationService->IsRunning()) == true)
		{
			_simulationService->Start();
		}
		ImGui::SameLine();
		if (ImGui::IconButton(_stopButtonIcon, 2, _simulationService->IsRunning()) == true)
		{
			_simulationService->Stop();
		}

		/* setup the dockspace */
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiWindowFlags_NoBackground);

		/* render of the currently open views */
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			bool showFlag = *(it->second)->GetShowFlagRef();
			if (showFlag == true)
			{
				(it->second)->OnUpdate(ts);

				(it->second)->OnRender(ts, renderer);

				(it->second)->OnPreEditorRender();
				(it->second)->OnEditorRender();
				(it->second)->OnPostEditorRender();
			}
		}

		if (menu_help_demo_window_show == true)
		{
			ImGui::ShowDemoWindow();
		}

		ImGui::End();

		ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog)->OnGUI();

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

	void EditorLayer::OnSceneOpened(Events::EventArgs& args)
	{
		/* cast the event args to cor
		type */
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		/* get the current loaded project name */
		std::string projectName = _workspaceService->GetCurrentProject()->name;

		/* set the title */
		Application::Get().GetWindow().SetTitle("Wyrd Engine - " + projectName + " [" + evtArgs.scene->name + "] ");

		/* set the background color */
		Application::Get().color.r = evtArgs.scene->bgcolor.r;
		Application::Get().color.g = evtArgs.scene->bgcolor.g;
		Application::Get().color.b = evtArgs.scene->bgcolor.b;
	}

	bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		/* mouse button pressed events should be routed to the correct view */
		for (auto& it = _views.begin(); it != _views.end(); it++)
		{
			glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

			if ((it->second)->GetBoundary().ContainsPoint(mousePos) == true)
			{
				/* store the view as the event owner */
				_mouseEventOwner = (it->second);

				(it->second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		for (auto& it = _views.begin(); it != _views.end(); it++)
		{
			glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

			if ((it->second)->GetBoundary().ContainsPoint(mousePos) == true)
			{
				(it->second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		
		_simulationService->SetMousePosition(e.GetX(), e.GetY());

		for (auto& it = _views.begin(); it != _views.end(); it++)
		{
			if ((it->second)->GetBoundary().ContainsPoint({ e.GetX(), e.GetY() }) == true)
			{
				(it->second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		for (auto& it = _views.begin(); it != _views.end(); it++)
		{
			glm::vec2 mousePos = { io.MousePos.x, io.MousePos.y };

			if ((it->second)->GetBoundary().ContainsPoint(mousePos) == true)
			{
				(it->second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		if (_mouseEventOwner != nullptr)
			_mouseEventOwner->OnEvent(e);

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


		/* global key events */
		if (e.GetKeyCode() == GLFW_KEY_F9)
		{
			_simulationService->CompileAll();
		}

		if (_mouseEventOwner != nullptr)
			_mouseEventOwner->OnEvent(e);

		return false;
	}

	bool EditorLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() > 0 && e.GetKeyCode() < 0x10000)
			io.AddInputCharacter((unsigned short)e.GetKeyCode());

		return io.WantCaptureKeyboard;
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