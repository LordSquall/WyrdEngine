#include "Scene.h"

#include "framework\framework.h"

#include <malloc.h>
#include <memory.h>

using namespace OrisisEngine;

Scene::Scene()
{

}

Scene::Scene(Framework* framework)
{
	_framework = framework;
	_logger = _framework->GetLogger();
}

Scene::Scene(const Scene &obj)
{
}

Scene::~Scene()
{
}

void Scene::OnSceneLoaded() { }
void Scene::OnSceneClosed() { }