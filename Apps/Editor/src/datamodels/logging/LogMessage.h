#pragma once

/* Core includes */

/* Local includes */
namespace Osiris::Editor {
	
	enum class Severity
	{
		Info,
		Warning,
		Error,
		Debug
	};

	enum class LogType
	{
		All,
		Core,
		Code,
		User
	};
}