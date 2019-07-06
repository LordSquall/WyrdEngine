#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "editor/events/EditorEvents.h"

#include "editor/EditorPlugin.h"
#include "editor/support/IconLibrary.h"
#include "editor/support/Utils.h"

#define EVENT_FUNC(f)	std::bind(&##f, this, std::placeholders::_1)

namespace Osiris::Editor::Events
{
	class  EventsManager
	{
	public:
		static void Subscribe(EventType type, std::function<void(EventArgs&)> callback);

		static void Publish(EventType type, EventArgs& args);

	private:
		static std::map<EventType, std::vector<std::function<void(EventArgs&)>>> _eventChannels;
	};
}