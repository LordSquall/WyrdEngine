#pragma once

/* core includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/maths/BoundingBox.h>

namespace Wyrd::Editor
{
	struct WYRD_LIBRARY_API EditorComponent
	{
		BoundingBox inputBoundingBox;
		glm::mat4 modelMatrix;
	};
};