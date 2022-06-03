/* local includes */
#include "GizmoHandle.h"

namespace Wyrd::Editor
{
	bool GizmoHandle::ContainsPoint(glm::vec2 worldSpacePt)
	{
		glm::vec2 localws = glm::inverse(matrix) * glm::vec4(worldSpacePt, 0.0f, 1.0f);

		return area.ContainsPoint(localws);
	}
}