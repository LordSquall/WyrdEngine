#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/scene/SceneLayer3D.h"

namespace Osiris
{
	bool SceneLayer3D::Initialise()
	{
		return true;
	}

	void SceneLayer3D::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{

	}

	void SceneLayer3D::AssignScripts(Behaviour* behaviour)
	{

	}

	bool SceneLayer3D::ToJson(jsonxx::Object& object)
	{
		//TODO
		return true;
	}

	bool SceneLayer3D::FromJson(jsonxx::Object& object)
	{
		//TODO
		return true;
	}
}