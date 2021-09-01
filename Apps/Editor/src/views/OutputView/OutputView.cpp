#pragma once

#include "wyrdpch.h"

#include "OutputView.h"

#include "support/ImGuiUtils.h"
#include "imgui.h"

namespace Wyrd::Editor
{
	OutputView::OutputView(EditorLayer* editorLayer) : EditorViewBase("Output", editorLayer)
	{
		/* cache the service(s) */
		_ResourceService = ServiceManager::Get<ResourceService>();
		_EventService = ServiceManager::Get<EventService>();

		/* cache the icon pointers */
		_CodeIcon = _ResourceService->GetIconLibrary().GetIcon("common", "code_error");

		/* register from events */
		_EventService->Subscribe(Events::EventType::AddLogEntry, [this](Events::EventArgs& args)
			{
				Events::AddLogEntryArgs& evtArgs = (Events::AddLogEntryArgs&)args;

				_LogItems.push_back({ evtArgs.type, evtArgs.severity, evtArgs.msg });
			});

		_EventService->Subscribe(Events::EventType::ClearLogEntry, [this](Events::EventArgs& args)
			{
				Events::ClearLogEntryArgs& evtArgs = (Events::ClearLogEntryArgs&)args;

				if (evtArgs.type == LogType::All)
				{
					_LogItems.clear();
				}
				else
				{
					_LogItems.erase(
						std::remove_if(
							_LogItems.begin(),
							_LogItems.end(),
							[&](const LogItem& item) // lambda
							{ return item.type == evtArgs.type; }
						),
						_LogItems.end()
					);
				}
			});
	}

	OutputView::~OutputView() {}

	void OutputView::OnEditorRender()
	{
		ImGui::Checkbox("Info", &_ShowInfo);
		ImGui::SameLine();
		ImGui::Checkbox("Warnings", &_ShowWarnings);
		ImGui::SameLine();
		ImGui::Checkbox("Errors", &_ShowErrors);
		ImGui::SameLine();
		ImGui::Checkbox("Debug", &_ShowDebug);

		if (ImGui::BeginTable("outputLogTable", 4, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("File");
			ImGui::TableSetupColumn("Code");
			ImGui::TableSetupColumn("Message");
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			int rowIndexCnt = 0;
			for (int i = 0; i < _LogItems.size(); ++i)
			{
				ImGui::PushID(&_LogItems[i]);
				ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 1.2f);
				ImGui::TableNextColumn();

				switch (_LogItems[i].type)
				{
				case LogType::Core:
					DrawCoreLogItem(i, _LogItems[i]);
					break;
				case LogType::Code:
					DrawCodeLogItem(i, _LogItems[i]);
					break;
				case LogType::User:
					DrawUserLogItem(i, _LogItems[i]);
					break;
				}

				ImGui::PopID();

				rowIndexCnt++;
			}

			ImGui::EndTable();
		}
	}

	void OutputView::DrawCoreLogItem(int idx, const LogItem& item)
	{
		ImGui::Selectable(item.message.c_str());
	}

	void OutputView::DrawCodeLogItem(int idx, const LogItem& item)
	{
		ImVec4 textColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		/* Typically error message */
		// C:\Projects\GameProjects\RPG\Assets\scripts\MenuCursor.cs(49,20): error CS1525: Unexpected symbol `_Transform2DComp'
		//															^      ^       ^     ^
		//															m1     m2      m3    m4
		
		size_t m1 = item.message.find('(');
		size_t m2 = item.message.find(':', m1);
		size_t m3 = item.message.find("CS", m2);
		size_t m4 = item.message.find(':', m2 + 1);

		/* extract the file name */
		std::string filename = item.message.substr(0, m1);

		/* extract the type */
		std::string type = item.message.substr(m2 + 2, (m3 - m2) - 3);

		/* extract the code */
		std::string code = item.message.substr(m3, (m4 - m3));

		/* extract the message */
		std::string message = item.message.substr(m4 + 2, (item.message.length() - m4) - 2);

		if (type == "warning")
		{
			textColor = { 1.0, 1.0, 0.0, 1.0f };
		}
		else if (type == "error")
		{
			textColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		}
		
		ImGui::TextColored(textColor, type.c_str());
		ImGui::TableNextColumn();
		ImGui::TextColored(textColor, std::filesystem::path(filename).filename().filename().string().c_str());
		ImGui::TableNextColumn();
		ImGui::TextColored(textColor, code.c_str());
		ImGui::TableNextColumn();
		ImGui::TextColored(textColor, message.c_str());
	}

	void OutputView::DrawUserLogItem(int idx, const LogItem& item)
	{
		ImGui::Selectable(item.message.c_str());
	}
}