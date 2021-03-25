#pragma once

#include "osrpch.h"

#include "OutputView.h"

#include "support/ImGuiUtils.h"
#include "imgui.h"

namespace Osiris::Editor
{
	OutputView::OutputView(EditorLayer* editorLayer) : EditorViewBase("Output", editorLayer)
	{
		/* cache the service(s) */
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);

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

		if (ImGui::BeginTable("outputLogTable", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("Type");
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
		int assetsFolderLength = Utils::GetAssetFolder().length();
		std::string filename = item.message.substr(0, item.message.find('('));
		std::string p = item.message.substr(assetsFolderLength);

		if (ImGui::IconButton(_CodeIcon, idx, true, ImVec2(16, 16), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f)))
		{
			Utils::OpenFileWithSystem(filename);
		}

		ImGui::TableNextColumn();

		ImGui::Text(p.c_str());
	}

	void OutputView::DrawUserLogItem(int idx, const LogItem& item)
	{
		ImGui::Selectable(item.message.c_str());

	}
}