#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "events/EditorEvents.h"

#define EVENT_FUNC(f)	std::bind(&##f, this, std::placeholders::_1)

namespace Wyrd::Editor
{
	class EventService : public IService
	{
	public:
		EventService() {}
		~EventService() {}


		void Subscribe(Events::EventType type, std::function<void(Events::EventArgs&)> callback);
		void Publish(Events::EventType type, std::unique_ptr<Events::EventArgs> args, bool enforceMainThread = false);

		void OnUpdate();

	private:
		std::map<Events::EventType, std::vector<std::function<void(Events::EventArgs&)>>> _eventChannels;

		std::vector<std::pair<Events::EventType, std::unique_ptr<Events::EventArgs>>> _BackgroundEvents;
		
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
	};
}