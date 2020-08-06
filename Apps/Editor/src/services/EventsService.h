#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "events/EditorEvents.h"

#define EVENT_FUNC(f)	std::bind(&##f, this, std::placeholders::_1)

namespace Osiris::Editor
{
	class EventService : public IService
	{
	public:
		EventService() {}
		~EventService() {}


		void Subscribe(Events::EventType type, std::function<void(Events::EventArgs&)> callback);
		void Publish(Events::EventType type, std::shared_ptr<Events::EventArgs> args);

		void OnUpdate();

	private:
		std::map<Events::EventType, std::vector<std::function<void(Events::EventArgs&)>>> _eventChannels;

		std::vector<std::pair<Events::EventType, std::shared_ptr<Events::EventArgs>>> _BackgroundEvents;
		//std::thread::id _MainThreadId;
		//std::mutex _BackgroundEventLock;
		
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
	};
}