#include "wyrdpch.h"
#include "ServiceManager.h"

namespace Wyrd::Editor
{
	std::map<ServiceType, std::shared_ptr<IService>> ServiceManager::_Services;
}