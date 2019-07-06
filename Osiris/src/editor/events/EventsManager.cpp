#pragma once

#include "osrpch.h"
#include "EventsManager.h"

namespace Osiris::Editor::Events
{
	std::map<EventType, std::vector<std::function<void(EventArgs&)>>> EventsManager::_eventChannels = std::map<EventType, std::vector<std::function<void(EventArgs&)>>>();

	void EventsManager::Subscribe(EventType type, std::function<void(EventArgs&)> callback)
	{
		/* check to see if event channel is already present */
		std::map<EventType, std::vector<std::function<void(EventArgs&)>>>::iterator it = _eventChannels.find(type);

		if (it != _eventChannels.end())
		{
			/* channel found, add the endpoint to the channel */
			it->second.push_back(callback);
		}
		else
		{
			/* new event channel, add the channel and then the endpoint */
			if (_eventChannels.insert(std::pair<EventType, std::vector<std::function<void(EventArgs&)>>>(type, std::vector<std::function<void(EventArgs&)>>())).second == true)
			{
				_eventChannels[type].push_back(callback);
			}
		}
	}

	void EventsManager::Publish(EventType type, EventArgs& args)
	{
		for (auto endpoint : _eventChannels[type])
		{
			endpoint(args);
		}
	}
}