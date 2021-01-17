#pragma once

#include "core/export.h"

#include "EventsService.h"
#include "WorkspaceService.h"
#include "ResourceService.h"
#include "DialogService.h"
#include "SettingsService.h"
#include "SimulationService.h"

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
			Dialog,
			Settings,
			Simulation
		};
	public:
		static void StartServices()
		{
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Events,		std::make_shared<EventService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Resources,	std::make_shared<ResourceService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Simulation, std::make_shared<SimulationService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Workspace,	std::make_shared<WorkspaceService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Dialog,		std::make_shared<DialogService>()));
			_Services.insert(std::pair<Service, std::shared_ptr<IService>>(Service::Settings,	std::make_shared<SettingsService>()));

			/* Order matters!!! */
			_Services[Service::Events]->OnCreate();
			_Services[Service::Settings]->OnCreate();
			_Services[Service::Resources]->OnCreate();
			_Services[Service::Workspace]->OnCreate();
			_Services[Service::Dialog]->OnCreate();
			_Services[Service::Simulation]->OnCreate();
		}

		static void EndServices()
		{
			_Services[Service::Simulation]->OnDestroy();
			_Services[Service::Dialog]->OnDestroy();
			_Services[Service::Workspace]->OnDestroy();
			_Services[Service::Resources]->OnDestroy();
			_Services[Service::Settings]->OnDestroy();
			_Services[Service::Events]->OnDestroy();
		}

		template <class T>
		static std::shared_ptr<T> Get(Service serviceType) { return std::dynamic_pointer_cast<T>(_Services[serviceType]); }


		static const std::map<Service, std::shared_ptr<IService>>& GetServices() { return _Services; }

	private:
		static std::map<Service, std::shared_ptr<IService>> _Services;
	};
}