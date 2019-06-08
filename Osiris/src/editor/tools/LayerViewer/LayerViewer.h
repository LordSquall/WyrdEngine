#include "core/export.h"
#include "core/LayerStack.h"

#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class LayerViewer : public EditorPlugin
	{
	public:
		LayerViewer();
		~LayerViewer();

		void OnEditorRender() override;

		inline void SetLayerStack(LayerStack* layerStack){ _LayerStack = layerStack; }

	private:
		LayerStack* _LayerStack;
	};
}