#pragma once

#include "osrpch.h"
#include "EditorLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "services/ServiceManager.h"

#include "layers/EditorRenderer2DLayer.h"

#include "tools/PropertiesViewer/PropertiesViewer.h"
#include "tools/PreferencesViewer/PreferencesViewer.h"
#include "tools/LayerViewer/LayerViewer.h"
#include "tools/SpriteLayerEditor/SpriteLayerEditor.h"
#include "tools/AssetViewer/AssetViewer.h"

#include "platform/OpenGL/imgui_opengl_renderer.h"

#include "support/ImGuiUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <shellapi.h>

namespace Osiris::Editor
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };
	static ImFont*		s_defaultFont							= nullptr;
	Utils util;

	static EditorRenderer2DLayer* renderer2DLayer;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		Application& app = Application::Get();
		LayerStack* stack = app.GetLayerStack();

		m_plugins["Properties"] = std::make_shared<PropertiesViewer>();
		m_plugins["Sprite Layer Editor"] = std::make_shared<SpriteLayerEditor>();
		m_plugins["Asset Viewer"] = std::make_shared<AssetViewer>();
		//m_plugins["Preferences"] = std::make_shared<PreferencesViewer>();
		//m_plugins["Preferences"]->Close();

		util = Utils();

		/* cache services */
		_eventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);
		_settingsService = ServiceManager::Get<SettingsService>(ServiceManager::Service::Settings);
		_resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);
		_simulationService = ServiceManager::Get<SimulationService>(ServiceManager::Service::Simulation);

		/* cache icon resources */
		_playButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_play");
		_stopButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_stop");

		/* retrieve the renderer 2D layer */
		renderer2DLayer = (EditorRenderer2DLayer*)Application::Get().GetLayerStack()->FindLayer("Editor2DLayer");

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
		for (std::map<std::string, std::shared_ptr<EditorPlugin>>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
		{
			(it->second)->OnInitialise();
		}

		m_Time = 0.0f;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		io.Fonts->AddFontFromFileTTF("Montserrat-Regular.otf", 16.0f);

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

		/* Set style variables */
		style.WindowRounding = 0.0f;

		ImGui_ImplOpenGL3_Init("#version 410");

		if (_workspaceService->LoadProject(_settingsService->GetSetting("Project", "default", "")) == false)
		{
			ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog)->OpenDialog(Dialogs::CreateNewProject);
		}

		return true;
	}

	void EditorLayer::OnDetach()
	{

	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		_simulationService->Update(ts);
	}

	void EditorLayer::OnRender(Timestep ts, Renderer& renderer)
	{
		static bool menu_app_close_show = false;
		static bool menu_help_demo_window_show = false;
		static bool toolbar_settings_window_show = false;

		ImVec2 menubar_size;

		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);

		float time = (float)glfwGetTime();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(s_defaultFont);

		/* menu bar */
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::BeginMenu("Create New Project...")) {
				if (ImGui::MenuItem("New")) 
				{
					ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog)->OpenDialog(Dialogs::CreateNewProject);
				}
				if (ImGui::MenuItem("Open File", "Ctrl+O")) {
					OSR_INFO(util.OpenFileDialog("*.scene"));
				}
				if (ImGui::MenuItem("Open Folder")) {
					OSR_INFO(util.OpenFolderDialog());
				}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open Project"))
			{
				_workspaceService->LoadProject(util.OpenFileDialog(".oproj"));
			}
			if (ImGui::MenuItem("Open Project In Explorer", nullptr, nullptr, _workspaceService->IsProjectLoaded()))
			{
				ShellExecuteA(NULL, "open", Utils::GetAssetFolder().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			ImGui::Separator();

			bool fileOperationsFlag = _workspaceService->IsSceneLoaded() && _workspaceService->IsProjectLoaded();

			if (ImGui::MenuItem("New Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded())) {
				std::string filepath = util.SaveFileDialog("Scene Json Files", "*.scene");
				_workspaceService->SaveSceneAs(filepath);
				_workspaceService->LoadScene(filepath);
			}

			if (ImGui::MenuItem("Save Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded() && _workspaceService->IsSceneLoaded())) {
				if (_workspaceService->SaveScene() == true)
				{
					OSR_CORE_INFO("Saved Scene");
				}
			}

			if (ImGui::MenuItem("Save Scene As..", nullptr, nullptr, _workspaceService->IsProjectLoaded() && _workspaceService->IsSceneLoaded())) {
				std::string filepath = util.SaveFileDialog("Scene Json Files", "scene");
				if (_workspaceService->SaveSceneAs(filepath) == true)
				{
					OSR_CORE_INFO("Saved Scene As");
				}
			}

			if (ImGui::MenuItem("Open Scene", nullptr, nullptr, _workspaceService->IsProjectLoaded())) {
				std::string filepath = util.OpenFileDialog(".scene");
				if (_workspaceService->LoadScene(filepath) == true)
				{
					OSR_CORE_INFO("Opened Scene");
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

		if (ImGui::BeginMenu("Scene", true))
		{
			if (ImGui::MenuItem("Settings"))
			{
				toolbar_settings_window_show = !toolbar_settings_window_show;
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

		if (ImGui::BeginMenu("Export", true))
		{
			if (ImGui::MenuItem("Export Windows Executable") == true)
			{
				OSR_CORE_TRACE("Windows Export...");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help", true))
		{
			ImGui::MenuItem("Show Demo Window", NULL, &menu_help_demo_window_show);

			ImGui::EndMenu();
		}

		menubar_size = ImGui::GetWindowSize();

		ImGui::EndMainMenuBar();

		//ImGui::SetNextWindowSize(ImVec2(Application::Get().GetWindow().GetWidth(), 25.0f));
		ImGui::SetNextWindowSize(menubar_size);
		ImGui::SetNextWindowPos(ImVec2(0.0f, menubar_size.y));
		ImGui::Begin("Toolbar", 0, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	
		ImGui::SetCursorPosX(menubar_size.x * 0.5f);
		ImGui::PushID("Sim_Play_Btn");
		if (ImGui::IconButton(_playButtonIcon, ImVec2(16.0f, 16.0f)) == true)
		{
			_simulationService->Start();
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("Sim_Stop_Btn");
		if (ImGui::IconButton(_stopButtonIcon, ImVec2(16.0f, 16.0f)) == true)
		{
			_simulationService->Stop();
		}
		ImGui::PopID();

		ImGui::End();

		if (toolbar_settings_window_show == true)
		{
			std::string sceneName = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->GetLoadedScene()->name;

			ImGui::Begin("Scene Settings");

			if (ImGui::InputText("Scene Name", (char*)sceneName.c_str(), sceneName.length()) == true)
			{
				ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->GetLoadedScene()->name = sceneName;
			}

			ImGui::ColorEdit3("BG Color", &Application::Get().color[0]);

			ImGui::End();
		}


		for (auto const&[key, val] : ServiceManager::GetServices())
		{
			val->OnGUI();
		}


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

		renderer2DLayer->OnGUI();

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
		/* cast the event args to correct type */
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		/* get the current loaded project name */
		std::string projectName = _workspaceService->GetCurrentProject()->name;

		/* set the title */
		Application::Get().GetWindow().SetTitle("Osiris Engine - " + projectName + " [" + evtArgs.scene->name + "] ");

		/* set the background color */
		Application::Get().color[0] = evtArgs.scene->bgcolor[0];
		Application::Get().color[1] = evtArgs.scene->bgcolor[1];
		Application::Get().color[2] = evtArgs.scene->bgcolor[2];
	}

	bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return io.WantCaptureMouse;
	}

	bool EditorLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return io.WantCaptureMouse;
	}

	bool EditorLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return io.WantCaptureMouse;
	}

	bool EditorLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		return io.WantCaptureMouse;
	}

	bool EditorLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return io.WantCaptureKeyboard;
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return io.WantCaptureKeyboard;
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