#include "core/export.h"
#include "layers/Renderer2DLayer.h"

#include "services/ServiceManager.h"
#include "views/EditorViewBase.h"

namespace Osiris::Editor
{
	class SpriteLayerEditor : public EditorViewBase
	{
	public:
		SpriteLayerEditor(EditorLayer* editorLayer);
		~SpriteLayerEditor();

		void OnInitialise() override;
		void OnEditorRender() override;

	private:
		void OnSceneOpened(Events::EventArgs& args);

		void OnSelectedGameObjectChanged(Events::EventArgs& args);

		void AddGameObject();

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;
		int32_t _SelectedSprite;
		std::shared_ptr<Layer2D> _SelectedLayer2D;

		std::shared_ptr<Icon> _LayerRenameIcon;
	};
}