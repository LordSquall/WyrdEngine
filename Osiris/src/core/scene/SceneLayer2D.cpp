#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/scene/SceneLayer2D.h"
#include "core/pipeline/SpriteBatch.h"
#include "core/Resources.h"

namespace Osiris
{
	bool SceneLayer2D::Initialise()
	{
		/* initiailse the sprite batch */
		_SpriteBatch = std::move(std::make_unique<SpriteBatch>());

		_SpriteBatch->SetShader(Resources::Get().Shaders["Sprite"]);

		return true;
	}

	void SceneLayer2D::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{
		_SpriteBatch->Render(renderer, viewProjectionMat);
	}

	jsonxx::Object SceneLayer2D::ToJson()
	{
		jsonxx::Object object;

		/* name */
		object << "name" << _Name;

		/* uid */
		object << "uid" << _UID.str();

		/* game objects */
		jsonxx::Array gameObjects;
		for (auto& gameobject : GetGameObjects())
		{
			gameObjects << gameobject->ToJson();
		}
		object << "gameObjects" << gameObjects;

		return object;
	}

	bool SceneLayer2D::FromJson(jsonxx::Object& object)
	{
		Initialise();

		/* name */
		_Name = object.get<jsonxx::String>("name", "MISSING");

		/* uid */
		_UID = UID(object.get<jsonxx::String>("uid", "MISSING"));

		/* game objects */
		if (object.has<jsonxx::Array>("gameObjects") == true)
		{
			for (size_t i = 0; i < object.get<jsonxx::Array>("gameObjects").size(); i++)
			{
				std::unique_ptr<GameObject> newGameObject = std::make_unique<GameObject>("NewGameObject");
				newGameObject->FromJson(object.get<jsonxx::Array>("gameObjects").get<jsonxx::Object>((int)i));
				AddGameObject(std::move(newGameObject));
			}
		}
		return true;
	}
}