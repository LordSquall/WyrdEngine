#pragma once

#include "core/export.h"

#include "editor/services/IService.h"

#include "core/pipeline/Scene.h"

namespace Osiris::Editor
{
	class SceneService : public IService
	{
	public:
		SceneService() {}
		~SceneService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		// Getters and Setters
		inline std::shared_ptr<Scene> GetLoadedScene() { return _loadedScene; }
		inline std::string& GetLoadedScenePath() { return _loadedScenePath; }

		bool CreateNewScene();
		bool LoadScene(std::string& path);
		bool SaveScene();
		bool SaveScene(std::string& path);
		
	private:
		std::string _loadedScenePath;
		std::shared_ptr<Scene> _loadedScene;

		
	};
}