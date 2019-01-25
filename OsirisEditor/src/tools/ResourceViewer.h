#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"

namespace Osiris::Editor
{
	class ResourceViewer : public EditorPlugin
	{
	public:
		ResourceViewer();
		~ResourceViewer();
		
		void OnEditorRender() override;

		static bool showTextures;
		static bool showShaders;

	private:
		void ShowTextureDialog();
		void ShowShaderDialog();
	};
}