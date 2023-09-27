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
#include <misc/cpp/imgui_stdlib.h>


namespace Wyrd::Editor
{
	struct ImMemoryTableParams
	{
		int columnCount = 32;
		int elementSize = 32;
	};

	static ImMemoryTableParams sMemoryTableParams;

	void DrawMemoryTable(byte* data, size_t size, ImMemoryTableParams* params)
	{
		static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX;
		int elementByteCounter = 0;
		bool cellColorToggle = false;
		ImU32 cellColor = ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f));

		ImGui::InputInt("Column Count", &params->columnCount);

		if (ImGui::BeginTable("_memorytable", params->columnCount + 1, tableFlags))
		{
			/* Draw table headers */
			ImGui::TableSetupColumn("");

			for (int i = 0; i < params->columnCount; ++i)
			{
				char header[4];
				sprintf(header, "%d", i); 
				ImGui::TableSetupColumn(header);
			}
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("");

			size_t currentColumn = 1;
			for (size_t i = 0; i < size; ++i)
			{
				ImGui::TableSetColumnIndex(currentColumn);

				if (elementByteCounter == params->elementSize)
				{
					cellColor = cellColorToggle ? ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f)) : ImGui::GetColorU32(ImVec4(0.3f, 0.7f, 0.3f, 0.65f));
					cellColorToggle = !cellColorToggle;
					elementByteCounter = 0;
				}
				
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cellColor);

				ImGui::Text("%02X ", data[i]);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%c", data[i]);
					ImGui::EndTooltip();
				}
				currentColumn++;

				if (currentColumn == (params->columnCount + 1))
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("");
					currentColumn = 1;
				}


				elementByteCounter++;
			}

			ImGui::EndTable();
		}
	}

	SystemsDebugView::SystemsDebugView(EditorLayer* editorLayer) : EditorViewBase("Systems Debug View", editorLayer), _Scene(nullptr), _SelectedEntity(ENTITY_INVALID)
	{
		_ShowFlag = true;

		_EventService = ServiceManager::Get<EventService>();

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(SystemsDebugView::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(SystemsDebugView::OnSelectedEntityChanged));
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
				if (ImGui::BeginTabItem("ECS"))
				{
					if (ImGui::BeginTabBar("systems_debug_ecs_tabview"))
					{
						if (ImGui::BeginTabItem("Component Pools"))
						{
							static int sSelectedComponentPoolIdx = 0;
							static ImGuiTableFlags componentPoolsTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedSame;

							const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };

							if (ImGui::BeginCombo("Component", _Scene->componentPools[sSelectedComponentPoolIdx]->name.c_str()))
							{
								for (int i = 0; i < _Scene->componentPools.size(); ++i)
								{
									ImGui::PushID((void*)_Scene->componentPools[i]);
									if (ImGui::Selectable(_Scene->componentPools[i]->name.c_str(), i == sSelectedComponentPoolIdx))
									{
										sSelectedComponentPoolIdx = i;
									}
									ImGui::PopID();
								}
								ImGui::EndCombo();
							}

							if (ImGui::BeginTable("d_ecs_componentpools", 5, componentPoolsTableFlags))
							{
								size_t total = _Scene->componentPools[sSelectedComponentPoolIdx]->elementSize * _Scene->componentPools[sSelectedComponentPoolIdx]->count;

								ImGui::TableSetupColumn("Name");
								ImGui::TableSetupColumn("Id");
								ImGui::TableSetupColumn("ElementSize (bytes)");
								ImGui::TableSetupColumn("Count");
								ImGui::TableSetupColumn("Total");
								ImGui::TableHeadersRow();

								ImGui::TableNextRow();
								ImGui::TableSetColumnIndex(0);
								ImGui::Text("%s", _Scene->componentPools[sSelectedComponentPoolIdx]->name.c_str());
								ImGui::TableSetColumnIndex(1);
								ImGui::Text("%d", _Scene->componentPools[sSelectedComponentPoolIdx]->idx);
								ImGui::TableSetColumnIndex(2);
								ImGui::Text("%d", _Scene->componentPools[sSelectedComponentPoolIdx]->elementSize);
								ImGui::TableSetColumnIndex(3);
								ImGui::Text("%d", _Scene->componentPools[sSelectedComponentPoolIdx]->count);
								ImGui::TableSetColumnIndex(4);
								ImGui::Text("%d", total);

								ImGui::EndTable();
							}

							ImGui::BeginChild("d_esc_componentpool_data");

							if (sSelectedComponentPoolIdx != -1)
							{
								ComponentPool* cp = _Scene->componentPools[sSelectedComponentPoolIdx];

								sMemoryTableParams.elementSize = cp->elementSize;
								
								DrawMemoryTable((byte*)(&cp->data[0]), cp->count * cp->elementSize, &sMemoryTableParams);

							}
							ImGui::EndChild();

							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Entities"))
						{
							if (ImGui::BeginTable("d_ecs_entities", _Scene->componentPools.size() + 1))
							{
								ImGui::TableNextRow();
								for (size_t header = 0; header < _Scene->componentPools.size(); header++)
								{
									ImGui::TableSetColumnIndex(header+1);
									ImGui::Text(_Scene->componentPools[header]->name.c_str());
								}

								for (size_t row = 0; row < _Scene->entities.size(); row++)
								{
									ImGui::TableNextRow();
									ImGui::TableSetColumnIndex(0);
									ImGui::Text("Entity");
									for (size_t column = 1; column < _Scene->componentPools.size() + 1; column++)
									{
										ImGui::TableSetColumnIndex(column);
										ImGui::Text("x");
									}
								}
								ImGui::EndTable();
							}

							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}

					ImGui::EndTabItem();
				}
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
									ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
									if (ImGui::BeginTable("proptable", 2, flags))
									{
										ImGui::TableSetupColumn("Name");
										ImGui::TableSetupColumn("Type");
										ImGui::TableHeadersRow();
									
										for (auto& prop : *cls.second->Properties)
										{
											ImGui::TableNextRow();
											ImGui::TableNextColumn();
											ImGui::Text(prop.first.c_str());
											ImGui::TableNextColumn();
											ImGui::Text(prop.second->GetType().c_str());
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

	void SystemsDebugView::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);
		_SelectedEntity = evtArgs.entity;
	}
}