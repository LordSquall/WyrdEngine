#pragma once

#include "osrpch.h"

#include "core/export.h"
#include "LogMessage.h"

namespace Osiris::Editor
{
	class ScriptLogMsg : public LogMessage
	{
	public:
		ScriptLogMsg(const std::string script, const std::string msg, Serverity serverity) : script(script), msg(msg)
		{
			this->type = Script;
			this->serverity = serverity;
		}

		~ScriptLogMsg() {}

	public:
		const std::string script;
		const std::string msg;
	};
}