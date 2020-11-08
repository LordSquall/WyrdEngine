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

		void RenderGameObjectTreeNode(std::shared_ptr<GameObject>);

		/* Helper functions */
		std::shared_ptr<GameObject> CreateEmptyGameObject();

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		std::shared_ptr<GameObject> _SelectedGameObject;
		std::shared_ptr<Layer2D> _SelectedLayer2D;

		std::shared_ptr<Icon> _LayerRenameIcon;
	};
}