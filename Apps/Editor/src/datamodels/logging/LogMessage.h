#pragma once

/* Core includes */

/* Local includes */
namespace Wyrd::Editor {
	
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