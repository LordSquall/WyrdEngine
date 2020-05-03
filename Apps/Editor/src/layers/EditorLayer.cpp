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

#include "tools/PropertiesViewer/PropertiesViewer.h"
#include "tools/LayerViewer/LayerViewer.h"
#include "tools/SpriteLayerEditor/SpriteLayerEditor.h"
#include "tools/AssetViewer/AssetViewer.h"

#include "platform/OpenGL/imgui_opengl_renderer.h"

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

	EditorLayer::EditorLayer(std::string projectDirectory)
		: Layer("EditorLayer")
	{
		Application& app = Application::Get();
		LayerStack* stack = app.GetLayerStack();

		m_plugins["Properties"] = std::make_shared<PropertiesViewer>();

		std::shared_ptr<SpriteLayerEditor> spriteLayerEditor = std::make_shared<SpriteLayerEditor>();
		m_plugins["Sprite Layer Editor"] = spriteLayerEditor;

		m_plugins["Asset Viewer"] = std::make_shared<AssetViewer>();

		util = Utils();

		_eventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_projectService = ServiceManager::Get<ProjectService>(ServiceManager::Service::Project);
		_sceneService = ServiceManager::Get<SceneService>(ServiceManager::Service::Scene);

		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Editor::Events::EventType::SceneOpened, EVENT_FUNC(EditorLayer::OnSceneOpened));

		_projectService->LoadProject(projectDirectory);
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
		ImGuiStyle& style = ImGui::GetStyle();

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
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnRender(Timestep ts, Renderer& renderer)
	{
		static bool menu_app_close_show = false;
		static bool menu_help_demo_window_show = false;
		static bool toolbar_settings_window_show = false;

		ImVec2 menubar_size;

		_projectService = ServiceManager::Get<ProjectService>(ServiceManager::Service::Project);
		_sceneService = ServiceManager::Get<SceneService>(ServiceManager::Service::Scene);

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
				_projectService->LoadProject(util.OpenFileDialog(".oproj"));
			}
			if (ImGui::MenuItem("Open Project In Explorer", nullptr, nullptr, _projectService->IsProjectLoaded()))
			{
				ShellExecuteA(NULL, "open", Utils::GetAssetFolder().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			ImGui::Separator();

			bool fileOperationsFlag = _sceneService->IsSceneLoaded() && _projectService->IsProjectLoaded();

			if (ImGui::MenuItem("New Scene", nullptr, nullptr, _projectService->IsProjectLoaded())) {
				std::string filepath = util.SaveFileDialog("Scene Json Files", "*.scene");
				_sceneService->SaveScene(filepath);
				_sceneService->LoadScene(filepath);
			}

			if (ImGui::MenuItem("Save Scene As..", nullptr, nullptr, _projectService->IsProjectLoaded() && _sceneService->IsSceneLoaded())) {
				std::string filepath = util.SaveFileDialog("Scene Json Files", "scene");
				if (_sceneService->SaveScene(filepath) == true)
				{
					OSR_CORE_INFO("Saved Scene As");
				}
			}

			if (ImGui::MenuItem("Open Scene", nullptr, nullptr, _projectService->IsProjectLoaded())) {
				std::string filepath = util.OpenFileDialog(".scene");
				if (_sceneService->LoadScene(filepath) == true)
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
		
		ImGui::End();

		if (toolbar_settings_window_show == true)
		{
			std::string sceneName = ServiceManager::Get<SceneService>(ServiceManager::Scene)->GetLoadedScene()->name;

			ImGui::Begin("Scene Settings");

			if (ImGui::InputText("Scene Name", (char*)sceneName.c_str(), sceneName.length()) == true)
			{
				ServiceManager::Get<SceneService>(ServiceManager::Scene)->GetLoadedScene()->name = sceneName;
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
		std::string projectName = _projectService->GetCurrentProject()->name;

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