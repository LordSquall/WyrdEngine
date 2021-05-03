#pragma once

#include "wyrdpch.h"
#include "ServiceManager.h"

namespace Wyrd::Editor
{
	std::map<ServiceManager::Service, std::shared_ptr<IService>> ServiceManager::_Services;
}