/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>

/* local includes */
#include "EventsService.h"


/* external includes */

namespace Wyrd::Editor
{
	void EventService::Subscribe(Events::EventType type, std::function<void(Events::EventArgs&)> callback)
	{
		/* check to see if event channel is already present */
		std::map<Events::EventType, std::vector<std::function<void(Events::EventArgs&)>>>::iterator it = _eventChannels.find(type);

		if (it != _eventChannels.end())
		{
			/* channel found, add the endpoint to the channel */
			it->second.push_back(callback);
		}
		else
		{
			/* new event channel, add the channel and then the endpoint */
			if (_eventChannels.insert(std::pair<Events::EventType, std::vector<std::function<void(Events::EventArgs&)>>>(type, std::vector<std::function<void(Events::EventArgs&)>>())).second == true)
			{
				_eventChannels[type].push_back(callback);
			}
		}
	}

	void EventService::Publish(Events::EventType type, std::unique_ptr<Events::EventArgs> args, bool enforceMainThread)
	{
		/* we should make sure all events are processed on the main thread */
		if (std::this_thread::get_id() == Application::Get().GetMainThreadID() && enforceMainThread == true)
		{
			_BackgroundEvents.push_back({ type, std::move(args) });
		}
		else
		{
			for (auto endpoint : _eventChannels[type])
			{
				endpoint(*args);
			}
		}
	}

	void EventService::OnUpdate()
	{
		if (!_BackgroundEvents.empty())
		{
			for (auto& evt : _BackgroundEvents)
			{
				for (auto& endpoint : _eventChannels[evt.first])
				{
					endpoint(*evt.second);
				}
			}
			_BackgroundEvents.clear();
		}
	}

	void EventService::OnCreate()
	{
	}
	void EventService::OnDestroy()
	{
	}
}