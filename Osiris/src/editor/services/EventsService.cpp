#pragma once

#include "osrpch.h"
#include "EventsService.h"

namespace Osiris::Editor
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

	void EventService::Publish(Events::EventType type, Events::EventArgs& args)
	{
		for (auto endpoint : _eventChannels[type])
		{
			endpoint(args);
		}
	}
	void EventService::OnCreate()
	{
	}
	void EventService::OnDestroy()
	{
	}
}