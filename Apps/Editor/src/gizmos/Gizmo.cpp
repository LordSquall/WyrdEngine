/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/maths/Rect.h>

/* local includes */
#include "gizmos/Gizmo.h"
#include "views/SceneViewer/SceneViewer.h"
#include "datamodels/EditorComponents.h"

namespace Wyrd::Editor
{
	Gizmo::Gizmo(SceneViewer* sceneViewer, Entity entity) : _SceneViewer(sceneViewer), _Entity(entity)
	{
		_CameraController = sceneViewer->GetCamera();
	}
}