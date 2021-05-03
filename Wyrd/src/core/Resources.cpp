#include "wyrdpch.h"

#include "Resources.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/ScriptedClass.h"

namespace Wyrd {

	Resources* Resources::_Instance = nullptr;

	Resources::Resources()
	{
		_Instance = this;
	}
}