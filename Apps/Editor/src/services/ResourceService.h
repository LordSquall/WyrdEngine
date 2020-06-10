#pragma once

#include "core/export.h"

#include "services/IService.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/SceneRes.h"
#include "datamodels/resources/ScriptRes.h"
#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class ResourceService : public IService
	{
	public:
		enum Type
		{
			NONE = 0,
			UNKNOWN = 1,
			TEXTURE = 2,
			SHADER = 3,
			MODEL = 4,
			SCENE = 5,
			SCRIPT = 6
		};

	public:
		ResourceService() {}
		~ResourceService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		void AddResource(std::string& resourcePath, ResourceService::Type type);
		void ReloadResource(std::string& resourcePath);
		void DeleteResource(std::string& resourcePath);

		/* Icon Functions */
		inline IconLibrary& GetIconLibrary() { return _iconLibrary; }

		/* Texture Functions */
		inline std::map<uint32_t, std::shared_ptr<TextureRes>>& GetTextures() { return _textureResources; }
		std::shared_ptr<TextureRes> GetTextureByName(const std::string& name);
		std::shared_ptr<TextureRes> GetTextureByUID(const uint32_t uid);

		/* Scene Functions */
		inline std::map<uint32_t, std::shared_ptr<SceneRes>>& GetScenes() { return _sceneResources; }
		std::shared_ptr<SceneRes> GetSceneByName(const std::string& name);
		std::shared_ptr<SceneRes> GetSceneByUID(const uint32_t uid);

		/* Script Functions */
		inline std::map<uint32_t, std::shared_ptr<ScriptRes>>& GetScripts() { return _scriptResources; }
		std::shared_ptr<ScriptRes> GetScriptByName(const std::string& name);
		std::shared_ptr<ScriptRes> GetScriptByUID(const uint32_t uid);

		/* Helper Functions */
		Type DetermineType(const std::string& path);

	private:
		void OnProjectLoadedEvent(Events::EventArgs& args);
		void OnAddResourceEvent(Events::EventArgs& args);
		void OnDeleteResourceEvent(Events::EventArgs& args);
		void OnReloadResourceEvent(Events::EventArgs& args);

	private:
		IconLibrary _iconLibrary;

		std::map<uint32_t, std::shared_ptr<TextureRes>> _textureResources;
		std::map<uint32_t, std::shared_ptr<SceneRes>> _sceneResources;
		std::map<uint32_t, std::shared_ptr<ScriptRes>> _scriptResources;

		std::map<std::string, Type> _extensions;

		std::shared_ptr<TextureRes> _defaultTexture;
		std::shared_ptr<SceneRes> _defaultScene;
	};
}