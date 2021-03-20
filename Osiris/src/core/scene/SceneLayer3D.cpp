#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/scene/SceneLayer3D.h"

namespace Osiris
{
	void SceneLayer3D::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{

	}

	jsonxx::Object SceneLayer3D::ToJson()
	{
		jsonxx::Object object;
		//TODO
		return object;
	}

	bool SceneLayer3D::FromJson(jsonxx::Object& object)
	{
		//TODO
		return true;
	}
}