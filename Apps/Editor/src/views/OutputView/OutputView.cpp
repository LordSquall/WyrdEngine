#pragma once

#include "osrpch.h"

#include "OutputView.h"

#include "support/ImGuiUtils.h"
#include "imgui.h"

namespace Osiris::Editor
{
	OutputView::OutputView() : EditorViewBase("Output")
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

				_LogItems.push_back({ evtArgs.severity, evtArgs.msg });
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

		ImGui::PushItemWidth(-1);
		ImGui::ListBoxHeader("##label");

		for each (auto item in _LogItems)
		{
			ImGui::Selectable(item.message.c_str());

		}

		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
	}

	void OutputView::DrawLogItem(const LogItem& msg)
	{
		ImGui::Selectable(msg.message.c_str());
	}
}