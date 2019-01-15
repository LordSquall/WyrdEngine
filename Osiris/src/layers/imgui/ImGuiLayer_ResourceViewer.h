#include "core/export.h"
#include "core/Layer.h"

#include "ImGuiPlugin.h"

namespace Osiris
{
	class OSIRIS_API ImGuiLayer_ResourceViewer : public ImGuiPlugin
	{
	public:
		ImGuiLayer_ResourceViewer();
		~ImGuiLayer_ResourceViewer();
		
		void OnEditorRender() override;

		static bool showTextures;
		static bool showShaders;

	private:
		void ShowTextureDialog();
		void ShowShaderDialog();
	};
}