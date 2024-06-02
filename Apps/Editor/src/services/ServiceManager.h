#pragma once

#include "core/export.h"

#include "serial/LogEditorTypeFunctions.h"

#include "ServiceTypes.h"
#include "EventsService.h"
#include "WorkspaceService.h"
#include "ResourceService.h"
#include "DialogService.h"
#include "SettingsService.h"
#include "SimulationService.h"
#include "CoreSystemsService.h"

namespace Wyrd::Editor
{
	class ServiceManager
	{
	public:
		static void StartServices()
		{
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Events,			std::make_shared<EventService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Resources,		std::make_shared<ResourceService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Simulation,		std::make_shared<SimulationService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Workspace,		std::make_shared<WorkspaceService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Dialog,			std::make_shared<DialogService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::Settings,		std::make_shared<SettingsService>()));
			_Services.insert(std::pair<ServiceType, std::shared_ptr<IService>>(ServiceType::CoreSystems,	std::make_shared<CoreSystemsService>()));

			/* Order matters!!! */
			_Services[ServiceType::Events]->OnCreate();
			_Services[ServiceType::Settings]->OnCreate();
			_Services[ServiceType::Resources]->OnCreate();
			_Services[ServiceType::Workspace]->OnCreate();
			_Services[ServiceType::Dialog]->OnCreate();
			_Services[ServiceType::Simulation]->OnCreate();
			_Services[ServiceType::CoreSystems]->OnCreate();
		}

		static void EndServices()
		{
			_Services[ServiceType::CoreSystems]->OnDestroy();
			_Services[ServiceType::Simulation]->OnDestroy();
			_Services[ServiceType::Dialog]->OnDestroy();
			_Services[ServiceType::Workspace]->OnDestroy();
			_Services[ServiceType::Resources]->OnDestroy();
			_Services[ServiceType::Settings]->OnDestroy();
			_Services[ServiceType::Events]->OnDestroy();
		}

		template <class T>
		static std::shared_ptr<T> Get() { return std::dynamic_pointer_cast<T>(_Services[T::GetTypeID()]); }


		static const std::map<ServiceType, std::shared_ptr<IService>>& GetServices() { return _Services; }

	private:
		static std::map<ServiceType, std::shared_ptr<IService>> _Services;
	};
}