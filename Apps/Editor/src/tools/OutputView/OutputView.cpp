#pragma once

#include "osrpch.h"

#include "OutputView.h"

#include "datamodels/logging/InformationLogMsg.h"
#include "datamodels/logging/ScriptLogMsg.h"

#include "support/ImGuiUtils.h"
#include "imgui.h"

namespace Osiris::Editor
{
	OutputView::OutputView() : EditorPlugin("Output")
	{
		/* cache the service(s) */
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);

		/* cache the icon pointers */
		/* N/A */

		/* register from events */
		_EventService->Subscribe(Events::EventType::AddLogEntry, [this](Events::EventArgs& args)
			{
				Events::AddLogEntryArgs& evtArgs = (Events::AddLogEntryArgs&)args;

				_LogItems.push_back(evtArgs.msg);
			});
	}

	OutputView::~OutputView() {}

	void OutputView::OnEditorRender()
	{
		ImGui::Begin("Output");

		ImGui::Checkbox("Info", &_ShowInfo);
		ImGui::SameLine();
		ImGui::Checkbox("Warnings", &_ShowWarnings);
		ImGui::SameLine();
		ImGui::Checkbox("Errors", &_ShowErrors);
		ImGui::SameLine();
		ImGui::Checkbox("Debug", &_ShowDebug);
		

		ImGui::PushItemWidth(-1);
		ImGui::ListBoxHeader("##label");
		
		for each (auto & item in _LogItems)
		{
			if (((_ShowInfo == true) && (item->serverity == LogMessage::Info)) || ((_ShowWarnings == true) && (item->serverity == LogMessage::Warning)) ||
				((_ShowErrors == true) && (item->serverity == LogMessage::Error)) || ((_ShowDebug == true) && (item->serverity == LogMessage::Debug)))
			{
				switch (item->type)
				{
				case LogMessage::Type::Information: DrawInformationLogItem(*item); break;
				case LogMessage::Type::Script: DrawScriptLogItem(*item); break;
				default: break;
				}
			}
		}

		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();

		ImGui::End();
	}

	void OutputView::DrawInformationLogItem(const LogMessage& msg)
	{
		const InformationLogMsg& informationLogMsg = (const InformationLogMsg&)msg;
		ImGui::Selectable(informationLogMsg.msg.c_str());
	}

	void OutputView::DrawScriptLogItem(const LogMessage& msg)
	{
		const ScriptLogMsg& scriptLogMsg = (const ScriptLogMsg&)msg;
		ImGui::Selectable(scriptLogMsg.script.c_str());
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0, 0.0, 0.0, 1.0 });
		ImGui::Selectable(scriptLogMsg.msg.c_str());
		ImGui::PopStyleColor();
	}
}