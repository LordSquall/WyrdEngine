#pragma once

#include "osrpch.h"
#include "ServiceManager.h"

namespace Osiris::Editor
{
	std::map<ServiceManager::Service, std::shared_ptr<IService>> ServiceManager::_Services;
}