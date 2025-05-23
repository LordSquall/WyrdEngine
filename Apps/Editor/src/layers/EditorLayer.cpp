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
#include "views/EntityView/EntityView.h"
#include "views/HierarchyView/HierarchyView.h"
#include "views/Dialogs/PreferencesDialog.h"
#include "views/Dialogs/NewProjectDialog.h"
#include "views/SystemsDebugView/SystemsDebugView.h"
#include "views/MaterialEditorView/MaterialEditorView.h"
#include "platform/OpenGL/imgui_opengl_renderer.h"
#include "export/ExportManager.h"
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
#include <ImGuizmo.h>

#ifdef WYRD_PLATFORM_WINDOWS
#include <SOIL.h>
#include <shellapi.h>
#elif WYRD_PLATFORM_LINUX
#include <SOIL/SOIL.h>
#endif

namespace Wyrd::Editor
{
	static bool			s_MouseJustPressed[5]					= { false, false, false, false, false };
	static GLFWcursor*  s_MouseCursors[ImGuiMouseCursor_COUNT]	= { 0 };
	static ImFont*		s_defaultFont							= nullptr;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		Application& app = Application::Get();
		LayerStack* stack = app.GetLayerStack();

		/* register views */
		_views["Scene Viewer"] = std::make_shared<SceneViewer>(this);
		_views["Game Viewer"] = std::make_shared<GameViewer>(this);
		_views["Properties"] = std::make_shared<PropertiesViewer>(this);
		/*_views["Entities"] = std::make_shared<EntityView>(this);*/
		_views["Hierarchy"] = std::make_shared<HierarchyView>(this);
		_views["Asset Viewer"] = std::make_shared<AssetViewer>(this);
		_views["Output"] = std::make_shared<OutputView>(this);
		_views["Export View"] = std::make_shared<ExportView>(this);
		_views["Systems Debug"] = std::make_shared<SystemsDebugView>(this);
		_views["Material Editor"] = std::make_shared<MaterialEditorView>(this);

		/* cache services */
		_Event = ServiceManager::Get<EventService>();
		_Workspace = ServiceManager::Get<WorkspaceService>();
		_Settings = ServiceManager::Get<SettingsService>();
		_Resource = ServiceManager::Get<ResourceService>();
		_Simulation = ServiceManager::Get<SimulationService>();
		_Dialog = ServiceManager::Get<DialogService>();

		/* pass the scene viewer view to the simluation service */
		_Simulation->SetSceneViewer(_views["Scene Viewer"]);

		/* register for events */
		_Event->Subscribe(Events::EventType::OpenMaterialTool, [this](Events::EventArgs& args)
			{
				Events::OpenMaterialToolArgs* a = (Events::OpenMaterialToolArgs*)&args;
				std::shared_ptr<MaterialEditorView>* met = (std::shared_ptr<MaterialEditorView>*)&_views["Material Editor"];
				(*met)->SetMaterial(a->material);
				(*met)->Open();
			});
	}

	EditorLayer::~EditorLayer()
	{

	}

	bool EditorLayer::OnAttach()
	{
		/* load in icons sets */
		const std::string filepath = "res/icons/filesystem_icons.json";
		_Resource->GetIconLibrary().AddIconsFromFile(filepath);

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
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		/* add the editor gui font */
		io.Fonts->AddFontFromFileTTF("Montserrat-Regular.otf", 16.0f);

		/* set the background flags for imgui flags */
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendPlatformName = "imgui_openg3_renderer";

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

		/* load user preferences */
		LoadLayoutPreferences();

		/* attempt to load the default project otherwise asked the user to create a new one */
		if (_Workspace->LoadProject(_Settings->Get(CONFIG_PROJECT, CONFIG_PROJECT__DEFAULT, "")) == false)
		{
			_Dialog->OpenDialog(std::make_shared<NewProjectDialog>(this));
		}

		/* register for events */
		_Event->Subscribe(Events::EventType::CloseEditor, [this](Events::EventArgs& args)
			{
				Application::Get().Terminate();
			});
		_Event->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(EditorLayer::OnSceneOpened));

		return true;
	}

	void EditorLayer::OnDetach()
	{
		SaveLayoutPreferences();
		ImGui::SaveIniSettingsToDisk("imgui.ini");
	}
	
	void EditorLayer::OnUpdate(Timestep ts)
	{
		_Resource->OnUpdate();
		_Event->OnUpdate();
		_Simulation->OnUpdate();
		_Simulation->Update(ts);

		_Workspace->GetLoadedScene()->CleanUp();
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
		ImGuizmo::BeginFrame();

		/* push the editor font */
		ImGui::PushFont(s_defaultFont);

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

		/* menu menu start */
		ImGui::BeginMainMenuBar();

		static bool menu_help_demo_window_show = false;

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Switch Project ..."))
			{
				_Dialog->OpenDialog(std::make_shared<NewProjectDialog>(this));
			}
			if (ImGui::MenuItem("Open Loaded Project In Explorer", nullptr, nullptr, _Workspace->IsProjectLoaded()))
			{
				//ShellExecuteA(NULL, "open", Utils::GetAssetFolder().c_str(), NULL, NULL, SW_SHOWDEFAULT);
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
				if (ImGui::MenuItem((it->second)->GetName().c_str(), NULL, (it->second)->GetShowFlagRef()))
				{
					WYRD_CORE_TRACE("Test");
					ImGui::SaveIniSettingsToDisk("imgui.ini");
				}
			}


			ImGui::Separator();

			if (ImGui::BeginMenu("Layout"))
			{
				if (ImGui::MenuItem("Load..."))
				{
					std::string file = Utils::OpenFileDialog(".ini");
					ImGui::LoadIniSettingsFromDisk(file.c_str());
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Preferences"))
			{
				_Dialog->OpenDialog(std::make_shared<PreferencesDialog>(this));
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help", true))
		{
			ImGui::MenuItem("Show Demo Window", NULL, &menu_help_demo_window_show);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		/* primary control bar */
		const ImVec2 size(16.0f, 16.0f);

		//ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 64.0f) * 0.5f);
		//
		//if (ImGui::IconButton(_Resource->RetrieveIcon("common", "sim_play"), 1, !_Simulation->IsRunning() && _Simulation->IsAvailable(), size) == true)
		//{
		//	_Simulation->Start();
		//}
		//ImGui::SameLine();
		//if (ImGui::IconButton(_Resource->RetrieveIcon("common", "sim_pause"), 2, _Simulation->IsRunning() && _Simulation->IsAvailable(), size) == true)
		//{
		//	_Simulation->TogglePause();
		//}
		//ImGui::SameLine();
		//if (ImGui::IconButton(_Resource->RetrieveIcon("common", "sim_stop"), 3, _Simulation->IsRunning() && _Simulation->IsAvailable(), size) == true)
		//{
		//	_Simulation->Stop();
		//}
		//
		//ImGui::SameLine();
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) - (size.x * 3));
		
		if (ImGui::IconButton(_Resource->RetrieveIcon("common", "export_settings"), 4, true, size) == true)
		{
		}

		ImGui::SameLine();
		if (ImGui::IconButton(_Resource->RetrieveIcon("common", "export"), 5, true, size) == true)
		{
			_Workspace->SaveScene();
			ExportManager::Export();
			Utils::SystemExecute("..\\..\\bin\\Debug\\TestPlayer\\TestPlayer.exe --gamedir \"" + _Workspace->GetBuildsDirectory().string() + "/\"");
		}

		/* setup the dockspace */
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

		/* render of the currently open views */
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			bool showFlag = *(it->second)->GetShowFlagRef();
			if (showFlag == true)
			{
				(it->second)->OnUpdate(ts);
			}
		}

		/* render of the currently open views */
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			bool showFlag = *(it->second)->GetShowFlagRef();
			if (showFlag == true)
			{
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

		ServiceManager::Get<DialogService>()->OnGUI();

		ImGui::PopFont();

		//renderer.StartNamedSection("ImGui Editor");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		//renderer.EndNamedSection();
		
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressedEvent), nullptr);
		dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleasedEvent), nullptr);
		dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnMouseMovedEvent), nullptr);
		dispatcher.Dispatch<MouseScrolledEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnMouseScrolledEvent), nullptr);
		dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnKeyReleasedEvent), nullptr);
		dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent), nullptr);
		dispatcher.Dispatch<KeyTypedEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnKeyTypedEvent), nullptr);
		dispatcher.Dispatch<WindowResizeEvent>(WYRD_BIND_EVENT_FN(EditorLayer::OnWindowResizeEvent), nullptr);
	}

	void EditorLayer::OnSceneOpened(Events::EventArgs& args)
	{
		/* cast the event args to core type */
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		/* get the current loaded project name */
		std::string projectName = _Workspace->GetCurrentProject()->name;

		/* set the title */
		Application::Get().GetWindow().SetTitle("Wyrd Engine - " + projectName + " [" + evtArgs.scene->name + "] ");

		/* set the background color */
		Application::Get().color.r = evtArgs.scene->bgcolor.r;
		Application::Get().color.g = evtArgs.scene->bgcolor.g;
		Application::Get().color.b = evtArgs.scene->bgcolor.b;
	}

	bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		/* mouse button pressed events should be routed to the correct view */
		for (const auto& view : _views)
		{
			glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };
			if ((*view.second->GetShowFlagRef()))
			{
				if ((view.second)->GetBoundary().ContainsPoint(mousePos) == true)
				{
					/* store the view as the event owner */
					_mouseEventOwner = (view.second);

					(view.second)->OnEvent(e);
				}
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		for (const auto& view : _views)
		{
			glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

			if ((view.second)->GetBoundary().ContainsPoint(mousePos) == true)
			{
				(view.second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		
		_Simulation->SetMousePosition(e.GetX(), e.GetY());

		for (const auto& view : _views)
		{
			if ((view.second)->GetBoundary().ContainsPoint({ e.GetX(), e.GetY() }) == true)
			{
				(view.second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseScrolledEvent(MouseScrolledEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		for (const auto& view : _views)
		{
			glm::vec2 mousePos = { io.MousePos.x, io.MousePos.y };

			if ((view.second)->GetBoundary().ContainsPoint(mousePos) == true)
			{
				(view.second)->OnEvent(e);
			}
		}

		return false;
	}

	bool EditorLayer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = (ImGuiKey)GetPlatformKeyCode(e.GetKeyCode(), 0);
		io.AddKeyEvent(key, false);

		if (_mouseEventOwner != nullptr)
			_mouseEventOwner->OnEvent(e);

		return false;
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = (ImGuiKey)GetPlatformKeyCode(e.GetKeyCode(), 0);
		io.AddKeyEvent(key, true);

		//ImGuiIO& io = ImGui::GetIO();
		//ImGui::IsKeyDown((ImGuiKey)e.GetKeyCode());
		//io.KeysDown[e.GetKeyCode()] = true;
		//
		//io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		//io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		//io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		//io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];


		if (_mouseEventOwner != nullptr)
			_mouseEventOwner->OnEvent(e);


		/* global key events */
		if (e.GetKeyCode() == GLFW_KEY_F9)
		{
			_Simulation->CompileAll();
		}

		if (_mouseEventOwner != nullptr)
			_mouseEventOwner->OnEvent(e);

		return false;
	}

	bool EditorLayer::OnKeyTypedEvent(KeyTypedEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() > 0 && e.GetKeyCode() < 0x10000)
			io.AddInputCharacter((unsigned short)e.GetKeyCode());

		return io.WantCaptureKeyboard;
	}

	bool EditorLayer::OnWindowResizeEvent(WindowResizeEvent& e, void* data)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

	void EditorLayer::SaveLayoutPreferences()
	{
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			bool isShownPref = *(it->second->GetShowFlagRef());
			_Settings->SetPreference(std::to_string(isShownPref), "VIEWS", it->second->GetName());
		}
	}

	void EditorLayer::LoadLayoutPreferences()
	{
		for (std::map<std::string, std::shared_ptr<EditorViewBase>>::iterator it = _views.begin(); it != _views.end(); it++)
		{
			bool isShownPref = Utils::ToBool(_Settings->GetPreference("VIEWS", it->second->GetName(), "false"));
			isShownPref ? it->second->Open() : it->second->Close();
		}
	}

	int EditorLayer::GetPlatformKeyCode(int keycode, int scancode)
	{
		IM_UNUSED(scancode);
		switch (keycode)
		{
		case GLFW_KEY_TAB: return ImGuiKey_Tab;
		case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
		case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
		case GLFW_KEY_UP: return ImGuiKey_UpArrow;
		case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
		case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
		case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
		case GLFW_KEY_HOME: return ImGuiKey_Home;
		case GLFW_KEY_END: return ImGuiKey_End;
		case GLFW_KEY_INSERT: return ImGuiKey_Insert;
		case GLFW_KEY_DELETE: return ImGuiKey_Delete;
		case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
		case GLFW_KEY_SPACE: return ImGuiKey_Space;
		case GLFW_KEY_ENTER: return ImGuiKey_Enter;
		case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
		case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
		case GLFW_KEY_COMMA: return ImGuiKey_Comma;
		case GLFW_KEY_MINUS: return ImGuiKey_Minus;
		case GLFW_KEY_PERIOD: return ImGuiKey_Period;
		case GLFW_KEY_SLASH: return ImGuiKey_Slash;
		case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
		case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
		case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
		case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
		case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
		case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
		case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
		case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
		case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
		case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
		case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
		case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
		case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
		case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
		case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
		case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
		case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
		case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
		case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
		case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
		case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
		case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
		case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
		case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
		case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
		case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
		case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
		case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
		case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
		case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
		case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
		case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
		case GLFW_KEY_MENU: return ImGuiKey_Menu;
		case GLFW_KEY_0: return ImGuiKey_0;
		case GLFW_KEY_1: return ImGuiKey_1;
		case GLFW_KEY_2: return ImGuiKey_2;
		case GLFW_KEY_3: return ImGuiKey_3;
		case GLFW_KEY_4: return ImGuiKey_4;
		case GLFW_KEY_5: return ImGuiKey_5;
		case GLFW_KEY_6: return ImGuiKey_6;
		case GLFW_KEY_7: return ImGuiKey_7;
		case GLFW_KEY_8: return ImGuiKey_8;
		case GLFW_KEY_9: return ImGuiKey_9;
		case GLFW_KEY_A: return ImGuiKey_A;
		case GLFW_KEY_B: return ImGuiKey_B;
		case GLFW_KEY_C: return ImGuiKey_C;
		case GLFW_KEY_D: return ImGuiKey_D;
		case GLFW_KEY_E: return ImGuiKey_E;
		case GLFW_KEY_F: return ImGuiKey_F;
		case GLFW_KEY_G: return ImGuiKey_G;
		case GLFW_KEY_H: return ImGuiKey_H;
		case GLFW_KEY_I: return ImGuiKey_I;
		case GLFW_KEY_J: return ImGuiKey_J;
		case GLFW_KEY_K: return ImGuiKey_K;
		case GLFW_KEY_L: return ImGuiKey_L;
		case GLFW_KEY_M: return ImGuiKey_M;
		case GLFW_KEY_N: return ImGuiKey_N;
		case GLFW_KEY_O: return ImGuiKey_O;
		case GLFW_KEY_P: return ImGuiKey_P;
		case GLFW_KEY_Q: return ImGuiKey_Q;
		case GLFW_KEY_R: return ImGuiKey_R;
		case GLFW_KEY_S: return ImGuiKey_S;
		case GLFW_KEY_T: return ImGuiKey_T;
		case GLFW_KEY_U: return ImGuiKey_U;
		case GLFW_KEY_V: return ImGuiKey_V;
		case GLFW_KEY_W: return ImGuiKey_W;
		case GLFW_KEY_X: return ImGuiKey_X;
		case GLFW_KEY_Y: return ImGuiKey_Y;
		case GLFW_KEY_Z: return ImGuiKey_Z;
		case GLFW_KEY_F1: return ImGuiKey_F1;
		case GLFW_KEY_F2: return ImGuiKey_F2;
		case GLFW_KEY_F3: return ImGuiKey_F3;
		case GLFW_KEY_F4: return ImGuiKey_F4;
		case GLFW_KEY_F5: return ImGuiKey_F5;
		case GLFW_KEY_F6: return ImGuiKey_F6;
		case GLFW_KEY_F7: return ImGuiKey_F7;
		case GLFW_KEY_F8: return ImGuiKey_F8;
		case GLFW_KEY_F9: return ImGuiKey_F9;
		case GLFW_KEY_F10: return ImGuiKey_F10;
		case GLFW_KEY_F11: return ImGuiKey_F11;
		case GLFW_KEY_F12: return ImGuiKey_F12;
		case GLFW_KEY_F13: return ImGuiKey_F13;
		case GLFW_KEY_F14: return ImGuiKey_F14;
		case GLFW_KEY_F15: return ImGuiKey_F15;
		case GLFW_KEY_F16: return ImGuiKey_F16;
		case GLFW_KEY_F17: return ImGuiKey_F17;
		case GLFW_KEY_F18: return ImGuiKey_F18;
		case GLFW_KEY_F19: return ImGuiKey_F19;
		case GLFW_KEY_F20: return ImGuiKey_F20;
		case GLFW_KEY_F21: return ImGuiKey_F21;
		case GLFW_KEY_F22: return ImGuiKey_F22;
		case GLFW_KEY_F23: return ImGuiKey_F23;
		case GLFW_KEY_F24: return ImGuiKey_F24;
		default: return ImGuiKey_None;
		}
	}
}