#pragma once

#include "osrpch.h"
#include "core/export.h"

namespace Osiris::Editor
{
	class GameObject;

	class IPropertiesComponent
	{
	public:
		virtual void OnPropertyEditorDraw() = 0;

	public:
		std::shared_ptr<GameObject> OwnerGameObject;
	};
}