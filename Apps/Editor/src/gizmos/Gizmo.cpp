#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>

/* local includes */
#include "gizmos/Gizmo.h"
#include "views/SceneViewer/SceneViewer.h"

namespace Wyrd::Editor
{
	Gizmo::Gizmo(SceneViewer* sceneViewer, Shader* shader) : _SceneViewer(sceneViewer), _Shader(shader)
	{
		_CameraController = sceneViewer->GetCamera();
	}
}