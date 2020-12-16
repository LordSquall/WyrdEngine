#include "osrpch.h"

#include "Resources.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/ScriptedClass.h"

namespace Osiris {

	Resources* Resources::_Instance = nullptr;

	Resources::Resources()
	{
		_Instance = this;
	}
}