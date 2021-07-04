#pragma once

/* core includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/maths/Rect.h>

namespace Wyrd::Editor
{
	struct WYRD_LIBRARY_API EditorComponent
	{
		Rect inputArea;
	};
};