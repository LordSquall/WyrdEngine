#pragma once

#include "core/export.h"

#include "EventsService.h"
#include "WorkspaceService.h"
#include "ResourceService.h"
#include "DialogService.h"

namespace Osiris::Editor
{
	class ServiceManager
	{
	public:
		enum Service
		{
			Events = 0,
			Workspace,
			Resources,
			Dialog
		};
	public:
		static void StartServices()
		{
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Events,		std::make_shared<EventService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Resources,	std::make_shared<ResourceService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Workspace,	std::make_shared<WorkspaceService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Dialog,		std::make_shared<DialogService>()));

			/* Order matters!!! */
			_Services[Service::Events]->OnCreate();
			_Services[Service::Resources]->OnCreate();
			_Services[Service::Workspace]->OnCreate();
			_Services[Service::Dialog]->OnCreate();
		}

		template <class T>
		static std::shared_ptr<T> Get(Service serviceType) { return std::dynamic_pointer_cast<T>(_Services[serviceType]); }


		static const std::map<Service, std::shared_ptr<IService>>& GetServices() { return _Services; }

	private:
		static std::map<Service, std::shared_ptr<IService>> _Services;
	};
}