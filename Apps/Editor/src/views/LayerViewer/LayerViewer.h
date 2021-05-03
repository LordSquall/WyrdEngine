#include "core/export.h"
#include "core/LayerStack.h"

#include "views/EditorViewBase.h"

namespace Wyrd::Editor
{
	class LayerViewer : public EditorViewBase
	{
	public:
		LayerViewer(EditorLayer* editorLayer);
		~LayerViewer();

		void OnEditorRender() override;

		inline void SetLayerStack(LayerStack* layerStack){ _LayerStack = layerStack; }

	private:
		LayerStack* _LayerStack;
	};
}