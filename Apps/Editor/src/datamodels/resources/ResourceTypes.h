#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

namespace Wyrd::Editor
{
	enum ResourceType
	{
		NONE = 0,
		UNKNOWN = 1,
		TEXTURE = 2,
		SHADER = 3,
		MODEL = 4,
		SCENE = 5,
		SCRIPT = 6,
		FONT = 7,
		MATERIAL = 8,
		SHADERSTAGE = 9,
	};
	
	class Resource;
	class TextureRes;
	class ShaderRes;
	class ModelRes;
	class SceneRes;
	class ScriptRes;
	class FontTypeRes;
	class MaterialRes;
	class ShaderStageRes;

	typedef std::shared_ptr<Resource> ResourceRef;
	typedef std::shared_ptr<TextureRes> TextureResRef;
	typedef std::shared_ptr<ShaderRes> ShaderResRef;
	typedef std::shared_ptr<ModelRes> ModelResRef;
	typedef std::shared_ptr<SceneRes> SceneResRef;
	typedef std::shared_ptr<ScriptRes> ScriptResRef;
	typedef std::shared_ptr<FontTypeRes> FontResRef;
	typedef std::shared_ptr<MaterialRes> MaterialResRef;
	typedef std::shared_ptr<ShaderStageRes> ShaderStageResRef;
}
