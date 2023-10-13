#pragma once

#include "core/export.h"

#include "services/ServiceTypes.h"
#include "services/IService.h"
#include "datamodels/resources/ResourceFactory.h"
#include "events/EditorEvents.h"

namespace Wyrd::Editor
{
	class WorkspaceService;

	class ResourceService : public IService
	{
	public:
		ResourceService() {}
		~ResourceService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate() override;

		static ServiceType GetTypeID() { return ServiceType::Resources; }

		void AddResource(const std::filesystem::path& resourcePath, const UID uid);
		void ReloadResource(UID uid);
		void DeleteResource(UID uid);

		/* Icon Functions */
		inline IconLibrary& GetIconLibrary() { return _iconLibrary; }
		inline const Icon& RetrieveIcon(const std::string& library, const std::string& name) { return _iconLibrary.GetIcon(library, name); }

		template<class T>
		std::shared_ptr<T> GetResourceByID(const UID resourceId)
		{
			auto it = _resourceMap.find(resourceId);

			if (it != _resourceMap.end())
			{
				return std::dynamic_pointer_cast<T>(_resourceMap[resourceId]);
			}
			return nullptr;
		}

		std::shared_ptr<Resource> GetResourceByFilePath(const std::filesystem::path& path)
		{
			auto it = std::find_if(_resourceMap.begin(), _resourceMap.end(), [&](const auto& e) 
				{ 
					return e.second->GetPath().compare(path) == 0; 
				});

			if (it != _resourceMap.end())
			{
				return it->second;
			}
			return nullptr;
		}

		inline const std::map<UID, std::shared_ptr<Resource>> GetResources() { return _resourceMap; };

		inline const std::shared_ptr<TextureRes> GetDefaultTexture() { return _defaultTexture; };

		std::map<UID, std::shared_ptr<Resource>> GetResourcesByDir(const std::string& dir);

		std::shared_ptr<Resource> GetSceneResourceByName(const std::string& name);

		/* Helper Functions */
		bool CheckIgnored(const std::filesystem::path& path);
		ResourceType DetermineType(const std::filesystem::path& path);
		void BuildScripts();

	public:
		std::map<std::filesystem::path, UID> CachedFiles;

	private:
		bool LoadAssetCache(const std::string& filePath);

	private:
		void OnProjectLoadedEvent(Events::EventArgs& args);
		void OnAddFileEntryEvent(Events::EventArgs& args);
		void OnDeleteFileEntryEvent(Events::EventArgs& args);
		void OnReloadFileEntryEvent(Events::EventArgs& args);
		void OnModifiedFileEntryEvent(Events::EventArgs& args);
		void OnLoadAssetEvent(Events::EventArgs& args);

	private:
		IconLibrary _iconLibrary;
		std::map<UID, std::shared_ptr<Resource>> _resourceMap;
		std::map<std::string, ResourceType> _extensions;
		std::set<std::string> _ignoredExtensions;

		std::shared_ptr<TextureRes> _defaultTexture;
		std::shared_ptr<SceneRes> _defaultScene;

	private:
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::stack<Resource*> _loadableResources;
	};
}