#pragma once

/* local includes */
#include "support/IconLibrary.h"
#include "views/Toolbars/EditorToolbarBase.h"

/* external includes */
#include <glm/glm.hpp>

namespace Wyrd::Editor
{
	class SimulationToolbar : public EditorToolbarBase
	{
	public:
		SimulationToolbar();
		virtual ~SimulationToolbar() { }

		virtual void OnToolbarRender();

	protected:
		std::shared_ptr<Icon> _playButtonIcon;
		std::shared_ptr<Icon> _stopButtonIcon;
	};
}