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

		void AddResource(std::string& resourcePath);
		void ReloadResource(std::string& resourcePath);
		void DeleteResource(std::string& resourcePath);

		/* Icon Functions */
		inline IconLibrary& GetIconLibrary() { return _iconLibrary; }

		template<class T>
		std::shared_ptr<T> GetResourceByID(ResourceFactory::Type type, const uint32_t resourceId)
		{
			auto it = _resourceMap[type].find(resourceId);

			if (it != _resourceMap[type].end())
			{
				return_resourceMap[type][resourceId];
			}
			return nullptr;
		}

		template<class T>
		std::shared_ptr<T> GetResourceByName(ResourceFactory::Type type, const std::string& name)
		{
			for (auto const [key, val] : _resourceMap[type])
			{
				if (val->GetName().compare(name) == 0)
				{
					return std::dynamic_pointer_cast<T>(val);
				}
			}
			return nullptr;
		}

		std::map<uint32_t, std::shared_ptr<Resource>> GetResourcesOfType(ResourceFactory::Type type);

		/* Helper Functions */
		bool CheckIgnored(const std::string& path);
		ResourceFactory::Type DetermineType(const std::string& path);
		void BuildScripts();

	private:
		bool LoadAssetCache(const std::string& filePath);

	private:
		void OnProjectLoadedEvent(Events::EventArgs& args);
		void OnAddResourceEvent(Events::EventArgs& args);
		void OnDeleteResourceEvent(Events::EventArgs& args);
		void OnReloadResourceEvent(Events::EventArgs& args);

	private:
		IconLibrary _iconLibrary;

		std::map<ResourceFactory::Type, std::map<uint32_t, std::shared_ptr<Resource>>> _resourceMap;

		std::map<std::string, ResourceFactory::Type> _extensions;

		std::set<std::string> _ignoredExtensions;

		std::shared_ptr<TextureRes> _defaultTexture;
		std::shared_ptr<SceneRes> _defaultScene;
	};
}