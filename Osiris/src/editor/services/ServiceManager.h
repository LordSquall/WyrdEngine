#pragma once

#include "core/export.h"

#include "EventsService.h"
#include "ProjectService.h"
#include "SceneService.h"
#include "DialogService.h"

namespace Osiris::Editor
{
	class ServiceManager
	{
	public:
		enum Service
		{
			Events = 0,
			Project = 1,
			Scene = 2,
			Dialog = 3
		};
	public:
		static void StartServices()
		{
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Events, std::make_shared<EventService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Project, std::make_shared<ProjectService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Scene, std::make_shared<SceneService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Dialog, std::make_shared<DialogService>()));

			for (auto const&[key, val] : _Services)
			{
				val->OnCreate();
			}

		}

		template <class T>
		static T& Get(Service serviceType) { return (T&)*_Services[serviceType]; }


		static const std::map<Service, std::shared_ptr<IService>>& GetServices() { return _Services; }

	private:
		static std::map<Service, std::shared_ptr<IService>> _Services;
	};
}