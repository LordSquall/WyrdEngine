#pragma once

#include "osrpch.h"

#include "core/export.h"
#include "LogMessage.h"

namespace Osiris::Editor
{
	class InformationLogMsg : public LogMessage
	{
	public:
		InformationLogMsg(const std::string& msg) : msg(msg)
		{
			type = Information;
			serverity = serverity;
		}

		~InformationLogMsg() {}

	public:
		const std::string& msg;
	};
}