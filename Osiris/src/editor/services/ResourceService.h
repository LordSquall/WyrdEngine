#pragma once

#include "core/export.h"

#include "editor/services/IService.h"
#include "editor/datamodels/resources/Resource.h"
#include "editor/datamodels/resources/TextureRes.h"

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
			MODEL = 4
		};

	public:
		ResourceService() {}
		~ResourceService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		void AddResource(std::string& resourcePath, ResourceService::Type type);

		inline std::map<uint32_t, std::shared_ptr<TextureRes>>& GetTextures() { return _textureResources; }
		std::shared_ptr<TextureRes> GetTextureByName(const std::string& name);

	private:
		Type DetermineType(std::string& path);

		std::map<uint32_t, std::shared_ptr<TextureRes>> _textureResources;

		std::map<std::string, Type> _extensions;
	};
}