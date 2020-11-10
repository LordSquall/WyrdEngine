#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Application.h>
#include <layers/Renderer2DLayer.h>
#include <core/scene/Scene.h>
#include <core/scene/Layer2D.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/Transform3DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Osiris::Editor
{
	static void Read_Color(jsonxx::Array& json, glm::vec4* color)
	{
		color->r = (float)json.get<jsonxx::Number>(0);
		color->g = (float)json.get<jsonxx::Number>(1);
		color->b = (float)json.get<jsonxx::Number>(2);
		color->a = (float)json.get<jsonxx::Number>(3);
	}

	static void Write_Color(jsonxx::Object& object, std::string name, const glm::vec4* color)
	{
		jsonxx::Array colorArray;
		colorArray << color->r;
		colorArray << color->g;
		colorArray << color->b;
		colorArray << color->a;

		object << name << colorArray;
	}

	static void Read_Vec2(jsonxx::Array& json, glm::vec2* vector)
	{
		vector->x = (float)json.get<jsonxx::Number>(0);
		vector->y = (float)json.get<jsonxx::Number>(1);
	}

	static void Write_Vec2(jsonxx::Object& object, std::string name, const glm::vec2* vector)
	{
		jsonxx::Array vectorArray;
		vectorArray << vector->x;
		vectorArray << vector->y;

		object << name << vectorArray;
	}

	static void Read_Vec3(jsonxx::Array& json, glm::vec3* vector)
	{
		vector->x = (float)json.get<jsonxx::Number>(0);
		vector->y = (float)json.get<jsonxx::Number>(1);
		vector->z = (float)json.get<jsonxx::Number>(2);
	}

	static void Write_Vec3(jsonxx::Object& object, std::string name, float* vector)
	{
		jsonxx::Array vectorArray;
		vectorArray << vector[0];
		vectorArray << vector[1];
		vectorArray << vector[2];

		object << name << vectorArray;
	}

	static std::shared_ptr<Transform2DComponent> Read_Transform2DComponent(jsonxx::Object& json, std::shared_ptr<GameObject> owner)
	{
		/* create a component */
		std::shared_ptr<Osiris::Transform2DComponent> component = std::make_shared<Osiris::Transform2DComponent>(owner);

		/* configure properties */
		Read_Vec2(json.get<jsonxx::Array>("position"), &component->position);
		component->rotation = (float)json.get<jsonxx::Number>("rotation");
		Read_Vec2(json.get<jsonxx::Array>("scale"), &component->scale);

		return component;
	}

	static jsonxx::Object Write_Transform2DComponent(std::shared_ptr<IBaseComponent> transform2DComponent)
	{
		/* case the base object to a transform 2d */
		std::shared_ptr<Transform2DComponent> transform2D = std::static_pointer_cast<Transform2DComponent>(transform2DComponent);

		/* create new component json object */
		jsonxx::Object componentJson;

		/* base properties */
		componentJson << "Type" << (uint32_t)transform2DComponent->GetType();
		Write_Vec2(componentJson, "position", &transform2D->position);
		componentJson << "rotation" << transform2D->rotation;
		Write_Vec2(componentJson, "scale", &transform2D->scale);

		return componentJson;
	}

	static jsonxx::Object Write_Transform3DComponent(std::shared_ptr<IBaseComponent> transform3DComponent)
	{
		/* create new component json object */
		jsonxx::Object componentJson;

		/* base properties */
		componentJson << "Type" << transform3DComponent->GetType();

		return componentJson;
	}

	static std::shared_ptr<SpriteComponent> Read_SpriteComponent(jsonxx::Object& json, std::shared_ptr<GameObject> owner)
	{
		glm::vec2 spriteSize, spritePosition;

		/* create a component */
		std::shared_ptr<Osiris::SpriteComponent> component = std::make_shared<Osiris::SpriteComponent>(owner);

		/* configure properties */
		Read_Color(json.get<jsonxx::Array>("color"), &component->Color);
		Read_Vec2(json.get<jsonxx::Array>("size"), &spriteSize);
		Read_Vec2(json.get<jsonxx::Array>("position"), &spritePosition);

		component->Sprite->SetSize((int)spriteSize.x, (int)spriteSize.y);
		component->Sprite->SetPosition((int)spritePosition.x, (int)spritePosition.y);

		component->BaseTexture = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetTextureResourceByName(json.get<jsonxx::String>("baseTexture"))->GetTexture();

		owner->sprite = component;

		return component;
	}

	static jsonxx::Object Write_SpriteComponent(std::shared_ptr<IBaseComponent> spriteComponent)
	{
		/* case the base object to a sprite component */
		std::shared_ptr<SpriteComponent> sprite = std::static_pointer_cast<SpriteComponent>(spriteComponent);

		/* create new component json object */
		jsonxx::Object componentJson;

		//TODO - this is only require as sprite is a nested object, not sure if this is required
		glm::vec2 spriteSize = { sprite->Sprite->GetWidth(), sprite->Sprite->GetHeight() };
		glm::vec2 spritePosition = { sprite->Sprite->GetX(), sprite->Sprite->GetY() };

		/* base properties */
		componentJson << "Type" << (uint32_t)spriteComponent->GetType();

		Write_Color(componentJson, "color", &sprite->Color);
		Write_Vec2(componentJson, "size", &spriteSize);
		Write_Vec2(componentJson, "position", &spritePosition);

		componentJson << "baseTexture" << ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetTextureResourceByNativeID(sprite->BaseTexture->GetUID())->GetName();
		
		return componentJson;
	}

	static std::shared_ptr<ScriptComponent> Read_ScriptComponent(jsonxx::Object& json, std::shared_ptr<GameObject> owner)
	{
		/* create a component */
		std::shared_ptr<Osiris::ScriptComponent> component = std::make_shared<Osiris::ScriptComponent>(owner);

		/* retrieve json parameters */
		std::string script = json.get<jsonxx::String>("ScriptName", "");

		/* configure properties */
		std::shared_ptr<ScriptRes> scriptResource = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetScriptByName(script);
		if (scriptResource != nullptr)
		{
			component->Class = scriptResource->Script;
		}

		return component;
	}

	static jsonxx::Object Write_ScriptComponent(std::shared_ptr<IBaseComponent> scriptComponent)
	{
		/* case the base object to a script component */
		std::shared_ptr<ScriptComponent> script = std::static_pointer_cast<ScriptComponent>(scriptComponent);

		/* create new component json object */
		jsonxx::Object componentJson;

		/* base properties */
		componentJson << "Type" << (uint32_t)scriptComponent->GetType();

		if(script->Class != nullptr)
			componentJson << "ScriptName" << script->Class->GetName();

		return componentJson;
	}

	static std::shared_ptr<PhysicsComponent> Read_PhysicsComponent(jsonxx::Object& json, std::shared_ptr<GameObject> owner)
	{
		/* create a component */
		std::shared_ptr<Osiris::PhysicsComponent> component = std::make_shared<Osiris::PhysicsComponent>(owner);

		/* configure properties */
		component->SetIsStatic(json.get<jsonxx::Boolean>("IsStatic"));
		component->SetIsTrigger(json.get<jsonxx::Boolean>("IsTrigger"));
		component->SetUseSpriteBoundary(json.get<jsonxx::Boolean>("UseSpriteBoundary"));

		owner->physics = component;

		return component;
	}

	static jsonxx::Object Write_PhysicsComponent(std::shared_ptr<IBaseComponent> physicsComponent)
	{
		/* case the base object to a physics component */
		std::shared_ptr<PhysicsComponent> physics = std::static_pointer_cast<PhysicsComponent>(physicsComponent);

		/* create new component json object */
		jsonxx::Object componentJson;

		/* base properties */
		componentJson << "Type" << (uint32_t)physicsComponent->GetType();
		componentJson << "IsStatic" << physics->IsStatic();
		componentJson << "IsTrigger" << physics->IsTrigger();
		componentJson << "UseSpriteBoundary" << physics->UseSpriteBoundary();

		return componentJson;
	}

	static std::shared_ptr<GameObject> Read_GameObject(jsonxx::Object& json, std::shared_ptr<GameObject> parent)
	{
		/* create a new game object */
		std::shared_ptr<Osiris::GameObject> gameObject = std::make_shared<Osiris::GameObject>();

		/* set parent */
		gameObject->parent = parent;

		/* configure properties */
		gameObject->name = json.get<jsonxx::String>("name", "untitled");

		/* write requried components */
		gameObject->transform2D = Read_Transform2DComponent(json.get<jsonxx::Object>("transform2D"), gameObject);

		/* process component */
		if (json.has<jsonxx::Array>("components") == true)
		{
			for (size_t i = 0; i < json.get<jsonxx::Array>("components").size(); i++)
			{
				jsonxx::Object componentObj = json.get<jsonxx::Array>("components").get<jsonxx::Object>(i);
				SceneComponentType typeId = (SceneComponentType)(uint32_t)componentObj.get<jsonxx::Number>("Type");

				switch (typeId)
				{
					case SceneComponentType::SpriteRenderer:
						gameObject->components.push_back(Read_SpriteComponent(componentObj, gameObject));
						break;
					case SceneComponentType::ScriptComponent:
						gameObject->components.push_back(Read_ScriptComponent(componentObj, gameObject));
						break;
					case SceneComponentType::PhysicsComponent:
						gameObject->components.push_back(Read_PhysicsComponent(componentObj, gameObject));
						break;
				}
			}
		}

		/* process children */
		if (json.has<jsonxx::Array>("children") == true)
		{
			for (size_t i = 0; i < json.get<jsonxx::Array>("children").size(); i++)
			{
				jsonxx::Object childObj = json.get<jsonxx::Array>("children").get<jsonxx::Object>(i);
				gameObject->children.push_back(Read_GameObject(childObj, gameObject));
			}
		}

		return gameObject;
	}

	static jsonxx::Object Write_GameObject(std::shared_ptr<GameObject> gameObject)
	{
		/* create new Game Object json object */
		jsonxx::Object gameObjectJson;

		/* base properties */
		gameObjectJson << "name" << gameObject->name;

		/* write requried components */
		gameObjectJson << "transform2D" << Write_Transform2DComponent(gameObject->transform2D);

		/* write components */
		jsonxx::Array componentsJson;
		for (size_t i = 0; i < gameObject->components.size(); i++)
		{
			switch (gameObject->components[i]->GetType())
			{
				case SceneComponentType::SpriteRenderer:
					componentsJson << Write_SpriteComponent(gameObject->components[i]);
					break;
				case SceneComponentType::ScriptComponent:
					componentsJson << Write_ScriptComponent(gameObject->components[i]);
					break;
				case SceneComponentType::PhysicsComponent:
					componentsJson << Write_PhysicsComponent(gameObject->components[i]);
					break;
			}
		}
		gameObjectJson << "components" << componentsJson;

		/* write child gameobjects */
		jsonxx::Array childrenJson;
		for (auto& child : gameObject->children)
		{
			childrenJson << Write_GameObject(child);
		}
		gameObjectJson << "children" << childrenJson;

		return gameObjectJson;
	}

	static std::shared_ptr<Layer2D> Read_Layer2D(jsonxx::Object& json)
	{
		/* create a new layer2D object */
		std::shared_ptr<Osiris::Layer2D> layer2D = std::make_shared<Osiris::Layer2D>();

		/* configure properties */
		layer2D->name = json.get<jsonxx::String>("name", "untitled");

		/* process game objects */
		if (json.has<jsonxx::Array>("gameObjects") == true)
		{
			for (size_t i = 0; i < json.get<jsonxx::Array>("gameObjects").size(); i++)
			{
				std::shared_ptr<Osiris::GameObject> gameObject = Read_GameObject(json.get<jsonxx::Array>("gameObjects").get<jsonxx::Object>(i), nullptr);

				// store the layer reference in the gameobject
				gameObject->layer = layer2D;

				layer2D->AddChild(gameObject);
			}
		}

		return layer2D;
	}

	static jsonxx::Object Write_Layer2D(std::shared_ptr<Layer2D> layer2D)
	{
		/* create new Layer2D json object */
		jsonxx::Object layer2DJson;

		/* base properties */
		layer2DJson << "name" << layer2D->name;
		
		/* write game objects */
		jsonxx::Array gameObjectJson;
		for (size_t i = 0; i < layer2D->children.size(); i++)
		{
			gameObjectJson << Write_GameObject(layer2D->children[i]);
		}

		layer2DJson << "gameObjects" << gameObjectJson;

		return layer2DJson;
	}

	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				/*  background color */
				if (o.has<jsonxx::Array>("bgcolor") == true)
					Read_Color(o.get<jsonxx::Array>("bgcolor"), &scene.bgcolor);

				/* camera position */
				if (o.has<jsonxx::Array>("cameraPosition") == true)
					Read_Vec3(o.get<jsonxx::Array>("cameraPosition"), &scene.cameraPosition);

				/* camera Zoom */
				if (o.has<jsonxx::Number>("cameraZoom") == true)
					scene.cameraZoom = o.get<jsonxx::Number>("cameraZoom");

				/* layer 2D */
				if (o.has<jsonxx::Array>("layers2D") == true)
				{
					for (size_t i = 0; i < o.get<jsonxx::Array>("layers2D").size(); i++)
					{
						scene.layers2D.push_back(Read_Layer2D(o.get<jsonxx::Array>("layers2D").get<jsonxx::Object>(i)));
					}
				}

				for each (auto layers in scene.layers2D)
				{
					for each (auto gameobject in layers->children)
					{
						gameobject->transform2D->Recalculate();

						for each (auto component in gameobject->components)
						{
							component->Recalculate();
						}
					}
				}
			}
			else
			{
				result = FileMalformed;
			}
		}
		else
		{
			result = FileNotFound;
		}

		return result;
	}

	SceneLoader::Result SceneLoader::Save(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		/*  background color */
		Write_Color(o, "bgcolor", &scene.bgcolor);

		/* camera position */
		Write_Vec3(o, "cameraPosition", &scene.cameraPosition[0]);
		
		/* camera Zoom */
		o << "cameraZoom" << scene.cameraZoom;

		/* layer 2D */
		jsonxx::Array layer2DJson;

		for (size_t i = 0; i < scene.layers2D.size(); i++)
		{
			/* create new Layer2D object array */
			layer2DJson << Write_Layer2D(scene.layers2D[i]);
		}

		o << "layers2D" << layer2DJson;

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
