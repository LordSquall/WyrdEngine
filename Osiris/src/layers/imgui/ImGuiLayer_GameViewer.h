#include "core/export.h"
#include "core/Layer.h"

#include "ImGuiPlugin.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer_GameViewer : public ImGuiPlugin
	{
	public:
		ImGuiLayer_GameViewer();
		~ImGuiLayer_GameViewer();
		
		void OnEditorRender() override;

	};
}