#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>

namespace Osiris::Editor
{
	class IPropertiesView
	{
	public:
		virtual void OnPropertyEditorDraw() = 0;
		virtual void OnSceneViewerDraw(const glm::mat4& viewProjectMatrix) {};

		virtual const std::string GetName() = 0;

		inline const bool GetEnabledControls() const { return _EnableControls; }

	protected:
		bool _EnableControls = true;
	};
}