#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "editor/services/ServiceManager.h"
#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class SpriteLayerEditor : public EditorPlugin
	{
	public:
		SpriteLayerEditor();
		~SpriteLayerEditor();

		void OnInitialise() override;
		void OnEditorRender() override;

	private:
		void OnSceneOpened(Events::EventArgs& args);

	private:
		SceneService* _SceneService;
		int32_t _SelectedSprite;
		std::shared_ptr<Layer2D> _SelectedLayer2D;
	};
}