#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/systems/ISystem.h>

/* local includes */
#include "services/ServiceTypes.h"
#include "services/IService.h"
#include "events/EditorEvents.h"

namespace Wyrd::Editor
{
	class CoreSystemsService : public IService
	{
	public:
		~CoreSystemsService() {}

		void RegisterSystem(const std::string& name, ISystem* system);

		template <class T>
		T* GetSystem(const std::string& name) { return (T*)_Systems[name]; }

		static ServiceType GetTypeID() { return ServiceType::CoreSystems; }

	private:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		std::map<std::string, ISystem*> _Systems;
	};
}