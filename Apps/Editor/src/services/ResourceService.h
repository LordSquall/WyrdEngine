#pragma once

#include "core/export.h"

#include "services/IService.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/ResourceFactory.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/SceneRes.h"
#include "datamodels/resources/ScriptRes.h"
#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class ResourceService : public IService
	{
	public:
		ResourceService() {}
		~ResourceService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate() override;

		void AddResource(std::string& resourcePath, const UID uid);
		void ReloadResource(UID uid);
		void DeleteResource(UID uid);

		/* Icon Functions */
		inline IconLibrary& GetIconLibrary() { return _iconLibrary; }

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

		inline const std::map<UID, std::shared_ptr<Resource>> GetResources() { return _resourceMap; };

		std::shared_ptr<ScriptRes> GetScriptResourceByName(const std::string& name);

		inline const std::shared_ptr<TextureRes> GetDefaultTexture() { return _defaultTexture; };

		std::map<UID, std::shared_ptr<Resource>> GetResourcesByDir(const std::string& dir);

		/* Helper Functions */
		bool CheckIgnored(const std::string& path);
		ResourceType DetermineType(const std::string& path);
		void BuildScripts();

	public:
		std::map<std::string, UID> CachedFiles;

	private:
		bool LoadAssetCache(const std::string& filePath);

	private:
		void OnProjectLoadedEvent(Events::EventArgs& args);
		void OnAddFileEntryEvent(Events::EventArgs& args);
		void OnDeleteFileEntryEvent(Events::EventArgs& args);
		void OnReloadFileEntryEvent(Events::EventArgs& args);
		void OnLoadAssetEvent(Events::EventArgs& args);

	private:
		IconLibrary _iconLibrary;

		std::map<UID, std::shared_ptr<Resource>> _resourceMap;

		std::map<std::string, ResourceType> _extensions;

		std::set<std::string> _ignoredExtensions;

		std::shared_ptr<TextureRes> _defaultTexture;
		std::shared_ptr<SceneRes> _defaultScene;

	private:
		std::stack<Resource*> _loadableResources;
	};
}