#ifndef _SCENE_H_
#define _SCENE_H_

#include "export.h"

using namespace std;

namespace OrisisEngine
{
	class Framework;
	class ILogger;

	class Scene {
	public:
		Scene();
		Scene(Framework* framework);
		Scene(const Scene &obj);
		~Scene();

		virtual void OnSceneLoaded();
		virtual void OnSceneClosed();

	protected:
		Framework* _framework;
		ILogger* _logger;
	};
}

#endif