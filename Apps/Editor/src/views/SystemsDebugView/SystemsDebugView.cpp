/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/behaviour/ScriptedCustomObject.h>

/* local include */
#include "SystemsDebugView.h"

/* external includes */
#include <glm/glm.hpp>
#include <imgui.h>

namespace Wyrd::Editor
{
	SystemsDebugView::SystemsDebugView(EditorLayer* editorLayer) : EditorViewBase("Systems Debug View", editorLayer), _Scene(nullptr)
	{

		_EventService = ServiceManager::Get<EventService>();

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(SystemsDebugView::OnSceneOpened));
	}

	SystemsDebugView::~SystemsDebugView() {}

	void SystemsDebugView::OnUpdate(Timestep ts)
	{
		
	}

	void SystemsDebugView::OnRender(Timestep ts, Renderer& renderer)
	{

	}

	void SystemsDebugView::OnEvent(Event& event)
	{
	}

	void SystemsDebugView::OnEditorRender()
	{
		if (_Scene)
		{
			if (ImGui::BeginTabBar("systems_debug_tabview", 0))
			{
				if (ImGui::BeginTabItem("Behaviour"))
				{
					Behaviour& behaviour = Application::Get().GetBehaviour();

					if (ImGui::CollapsingHeader("Scripted Classes"))
					{
						const auto& scriptedClasses = behaviour.GetScriptedClasses();

						for (auto& cls : scriptedClasses)
						{
							if (ImGui::TreeNode(cls.first.c_str()))
							{
								ImGui::TreePop();
							}
						}
					}

					if (ImGui::CollapsingHeader("Custom Scripted Classes"))
					{
						const auto& customScriptedClasses = behaviour.GetScriptedCustomClasses();

						for (auto& cls : customScriptedClasses)
						{
							if (ImGui::TreeNode(cls.first.c_str()))
							{
								if (ImGui::TreeNode("Properties"))
								{
									//ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
									//if (ImGui::BeginTable("proptable", 2, flags))
									//{
									//	ImGui::TableSetupColumn("Name");
									//	ImGui::TableSetupColumn("Type");
									//	ImGui::TableHeadersRow();
									//
									//	for (auto& prop : cls.second->Properties)
									//	{
									//		ImGui::TableNextRow();
									//		ImGui::TableNextColumn();
									//		ImGui::Text(prop.first.c_str());
									//		ImGui::TableNextColumn();
									//		ImGui::Text(prop.second->GetTypeName().c_str());
									//	}
									//
									//	ImGui::EndTable();
									//}
									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Methods"))
								{
									ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
									if (ImGui::BeginTable("proptable", 1, flags))
									{
										ImGui::TableSetupColumn("Name");
										ImGui::TableHeadersRow();

										for (auto& method : cls.second->Methods)
										{
											ImGui::TableNextRow();
											ImGui::TableNextColumn();
											ImGui::Text(method.first.c_str());
										}
										ImGui::EndTable();
									}
									ImGui::TreePop();
								}

								ImGui::TreePop();
							}
						}
					}

					if (ImGui::CollapsingHeader("Runtime ECS Script Instances"))
					{
						const auto& customScriptedObjects = behaviour.GetESCScriptedCustomObjects();

						for (auto& so : customScriptedObjects)
						{
							// retrieve the Scripted Class
							std::shared_ptr<ScriptedClass> scriptedClass = behaviour.GetCustomClassByUID(so.first);
							if (ImGui::TreeNode(scriptedClass->GetName().c_str()))
							{
								for (auto& instance : so.second)
								{
									ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
									if (ImGui::BeginTable("scriptinstances_tbl", 1, flags))
									{

										ImGui::TableSetupColumn("Name");
										ImGui::TableHeadersRow();
										ImGui::TableNextRow();

										for (auto& prop : instance->GetProperties())
										{
											ImGui::TableNextColumn();
											ImGui::Text(prop.first.c_str());
										}
										ImGui::EndTable();
									}
								}

								ImGui::TreePop();
							}
						}

						//ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
						//if (ImGui::BeginTable("scriptinstances_tbl", 3, flags))
						//{
						//	ImGui::TableSetupColumn("Class Name");
						//	ImGui::TableSetupColumn("Guid");
						//	ImGui::TableSetupColumn("Count");
						//	ImGui::TableHeadersRow();
						//	ImGui::TableNextRow();
						//
						//	for (auto& so : customScriptedObjects)
						//	{
						//		// retrieve the Scripted Class
						//		auto& scriptedClass = behaviour.GetCustomClassByUID(so.first);
						//
						//		ImGui::TableNextColumn();
						//		ImGui::Text(scriptedClass->GetName().c_str());
						//		ImGui::TableNextColumn();
						//		ImGui::Text(so.first.str().c_str());
						//		ImGui::TableNextColumn();
						//		ImGui::Text("%d", so.second.size());
						//
						//		ImGui::TableNextColumn();
						//		if (ImGui::BeginTable("scriptinstancesinner_tbl", 3, flags))
						//		{
						//			ImGui::TableSetupColumn("Class Name");
						//			ImGui::TableSetupColumn("Guid");
						//			ImGui::TableSetupColumn("Count");
						//			ImGui::TableHeadersRow();
						//			ImGui::TableNextRow();
						//
						//			ImGui::EndTable();
						//		}
						//
						//	}
						//	ImGui::EndTable();
						//}
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
	}

	void SystemsDebugView::OnResize()
	{

	}

	void SystemsDebugView::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);
		_Scene = evtArgs.scene;
	}
}