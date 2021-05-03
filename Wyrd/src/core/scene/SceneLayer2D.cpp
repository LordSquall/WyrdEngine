#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/scene/SceneLayer2D.h"
#include "core/pipeline/SpriteBatch.h"
#include "core/Resources.h"

#include <jsonxx.h>

namespace Wyrd
{
	bool SceneLayer2D::Initialise()
	{
		/* initiailse the sprite batch */
		_SpriteBatch = std::move(std::make_unique<SpriteBatch>());

		_SpriteBatch->SetShader(Resources::Get().Shaders["Sprite"]);

		return true;
	}

	GameObject* SceneLayer2D::AddGameObject(std::unique_ptr<GameObject> gameObject) {
		gameObject->SetSceneLayer(this);
		_GameObjects.push_back(std::move(gameObject));

		return _GameObjects.back().get();
	}

	void SceneLayer2D::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{
		_SpriteBatch->Render(renderer, viewProjectionMat);
	}

	void SceneLayer2D::AssignScripts(Behaviour* behaviour)
	{
		for (auto& gameobject : _GameObjects)
		{
			gameobject->AssignScripts(behaviour);
		}
	}

	bool SceneLayer2D::ToJson(jsonxx::Object& object)
	{
		/* name */
		object << "name" << _Name;

		/* uid */
		object << "uid" << _UID.str();

		/* is visible */
		object << "isVisible" << _IsVisible;

		/* is active */
		object << "isActive" << _IsActive;

		/* game objects */
		jsonxx::Array gameObjects;
		for (auto& gameobject : GetGameObjects())
		{
			jsonxx::Object goObject;
			gameobject->ToJson(goObject);
			gameObjects << goObject;
		}
		object << "gameObjects" << gameObjects;

		return true;
	}

	bool SceneLayer2D::FromJson(jsonxx::Object& object)
	{
		Initialise();

		/* name */
		_Name = object.get<jsonxx::String>("name", "MISSING");

		/* uid */
		_UID = UID(object.get<jsonxx::String>("uid", "MISSING"));

		/* is visible */
		_IsVisible = object.get<jsonxx::Boolean>("isVisible", true);

		/* is active */
		_IsActive = object.get<jsonxx::Boolean>("isActive", true);

		/* game objects */
		if (object.has<jsonxx::Array>("gameObjects") == true)
		{
			for (size_t i = 0; i < object.get<jsonxx::Array>("gameObjects").size(); i++)
			{
				std::unique_ptr<GameObject> newGameObject = std::make_unique<GameObject>("NewGameObject");
				GameObject* gameObject = AddGameObject(std::move(newGameObject));

				gameObject->FromJson(object.get<jsonxx::Array>("gameObjects").get<jsonxx::Object>((int)i));
			}
		}
		return true;
	}
}