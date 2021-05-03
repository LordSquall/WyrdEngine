#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>


#define ONSCENERENDER_BIND(f)	std::bind(&##f, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

namespace Wyrd::Editor
{
	class IPropertiesView
	{
	public:
		virtual void Initialise() {};
		virtual void OnPropertyEditorDraw() = 0;
		virtual void OnSceneViewerDraw(const glm::mat4& viewProjectMatrix) {};

		virtual const std::string GetName() = 0;

		inline const bool GetEnabledControls() const { return _EnableControls; }

	protected:
		bool _EnableControls = true;
	};
}