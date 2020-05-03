#pragma once

#include "osrpch.h"

#include "core/export.h"

#include "support/IconLibrary.h"

namespace Osiris::Editor
{
	class IService
	{
	public:
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;

		virtual void OnGUI() {}
	};
}