#pragma once

#include "wyrdpch.h"

#include "core/export.h"

namespace Wyrd::Editor
{
	class IService
	{
	public:
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate() {};

		virtual void OnGUI() {}
	};
}