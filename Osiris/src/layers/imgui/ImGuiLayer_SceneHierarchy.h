#include "core/export.h"
#include "core/Layer.h"

#include "ImGuiPlugin.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer_SceneHierarchy : public ImGuiPlugin
	{
	public:
		ImGuiLayer_SceneHierarchy();
		~ImGuiLayer_SceneHierarchy();
		
		void OnEditorRender() override;

	};
}