#include "core/export.h"
#include "core/Layer.h"

#include "ImGuiPlugin.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer_SceneEditor : public ImGuiPlugin
	{
	public:
		ImGuiLayer_SceneEditor();
		~ImGuiLayer_SceneEditor();
		
		void OnEditorRender() override;

	};
}