#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class AssetViewer : public EditorPlugin
	{
	public:
		AssetViewer();
		~AssetViewer();

		void OnEditorRender() override;
	};
}