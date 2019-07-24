#pragma once

#include "core/export.h"

#include "editor/services/IService.h"

#include "editor/events/EditorEvents.h"

#define EVENT_FUNC(f)	std::bind(&##f, this, std::placeholders::_1)

namespace Osiris::Editor
{
	class EventService : public IService
	{
	public:
		EventService() {}
		~EventService() {}


		void Subscribe(Events::EventType type, std::function<void(Events::EventArgs&)> callback);
		void Publish(Events::EventType type, Events::EventArgs& args);

	private:
		std::map<Events::EventType, std::vector<std::function<void(Events::EventArgs&)>>> _eventChannels;

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
	};
}