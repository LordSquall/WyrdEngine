#pragma once
#include "osrpch.h"

/* Local includes */
#include "Scene.h"
#include "Layer2D.h"
#include "GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/SceneLayer2D.h"
#include "serial/TypeSerialisers.h"

namespace Osiris
{
	Scene::Scene() : name("Untitled Scene") 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}

	Scene::Scene(std::string name) : name(name) 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}


	void Scene::Update()
	{
		/* update each layer */
		for(auto& layer : _Layers)
		{
			//layer->Update();
		}
	}


	GameObject* Scene::FindGameObject(const UID uid)
	{
		/* update each layer */
		for (auto& layer : _Layers)
		{
			for (auto& gameObject : layer->GetGameObjects())
			{
				if (gameObject->uid == uid)
					return gameObject.get();

				auto found = gameObject->FindChild(uid);
				if (found != nullptr)
					return found;
			}
		}

		return nullptr;
	}


	void Scene::AssignScripts(Behaviour* behaviour)
	{
		for (auto& layer : _Layers)
		{
			layer->AssignScripts(behaviour);
		}
	}


	const std::unique_ptr<SceneLayer>& Scene::AddLayer(std::unique_ptr<SceneLayer> layer)
	{
		_Layers.push_back(std::move(layer));
		return _Layers.back();
	}
	
	void Scene::RemoveLayer(const UID& uid)
	{
		std::vector<std::unique_ptr<SceneLayer>>::iterator layerItr =
			find_if(_Layers.begin(), _Layers.end(),
				[&](std::unique_ptr<SceneLayer>& obj) { return obj->GetUID() == uid; }
		);

		_Layers.erase(std::remove(_Layers.begin(), _Layers.end(), *layerItr));
	}


	CameraComponent* Scene::GetPrimaryCamera()
	{
		for (auto& layer : _Layers)
		{
			for (auto& gameObject : layer->GetGameObjects())
			{
				CameraComponent* camera = gameObject->FindCameraComponent(_ScenePrimaryCamera);
				if (camera != nullptr)
					return camera;
			}
		}

		return nullptr;
	}

	jsonxx::Object Scene::ToJson()
	{
		jsonxx::Object object;

		/* background color */
		object << "bgcolor" << bgcolor;

		/* camera position */
		object << "cameraPosition" << cameraPosition;

		/* camera zoom */
		object << "cameraZoom" << cameraZoom;

		/* scene layers */
		jsonxx::Array sceneLayers;
		for (auto& layer : _Layers)
		{
			jsonxx::Object layerObject;
			layer->ToJson(layerObject);
			sceneLayers << layerObject;
		}
		object << "sceneLayers" << sceneLayers;

		object << "primaryCameraUID" << _ScenePrimaryCamera.str();

		return object;
	}

	bool Scene::FromJson(jsonxx::Object& object)
	{
		/* background color */
		if (object.has<jsonxx::Array>("bgcolor"))
			bgcolor << object.get<jsonxx::Array>("bgcolor");

		/* camera position */
		if (object.has<jsonxx::Array>("cameraPosition"))
			cameraPosition << object.get<jsonxx::Array>("cameraPosition");

		/* camera zoom */
		if (object.has<jsonxx::Number>("cameraZoom"))
			cameraZoom = object.get<jsonxx::Number>("cameraZoom");

		/* layer 2D */
		if (object.has<jsonxx::Array>("sceneLayers") == true)
		{
			for (size_t i = 0; i < object.get<jsonxx::Array>("sceneLayers").size(); i++)
			{
				std::unique_ptr<SceneLayer2D> newLayer = std::make_unique<SceneLayer2D>("NewLayer");
				newLayer->FromJson(object.get<jsonxx::Array>("sceneLayers").get<jsonxx::Object>((int)i));
				AddLayer(std::move(newLayer));
			}
		}

		/* primary camera uid*/
		if (object.has<jsonxx::String>("primaryCameraUID"))
			_ScenePrimaryCamera = UID(object.get<jsonxx::String>("primaryCameraUID"));

		return true;
	}
}