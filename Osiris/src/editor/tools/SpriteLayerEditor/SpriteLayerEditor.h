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

		void AddSpriteGO();

	private:
		std::shared_ptr<SceneService> _SceneService;
		std::shared_ptr<ResourceService> _ResourceService;
		int32_t _SelectedSprite;
		std::shared_ptr<Layer2D> _SelectedLayer2D;
	};
}