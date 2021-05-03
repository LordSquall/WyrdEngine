#include "core/export.h"

#include "services/ServiceManager.h"
#include "views/EditorViewBase.h"
#include "events/KeyEvent.h"

namespace Wyrd::Editor
{
	class SpriteLayerEditor : public EditorViewBase
	{
	public:
		SpriteLayerEditor(EditorLayer* editorLayer);
		~SpriteLayerEditor();

		void OnInitialise() override;
		void OnEditorRender() override;

		void OnEvent(Event& event) override;

			private:
		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedGameObjectChanged(Events::EventArgs& args);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);

		void RenderGameObjectTreeNode(std::unique_ptr<GameObject>& gameObject);


	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		std::shared_ptr<Layer2D> _SelectedLayer2D;		// Deprecate

		GameObject* _SelectedGameObject;
		SceneLayer* _SelectedSceneLayer;
		uint32_t _SelectedSceneLayerIdx;
		uint32_t _SelectedGameObjectIdx;

		std::shared_ptr<Icon> _Layer2DIcon;
		std::shared_ptr<Icon> _GameObjectIcon;
		std::shared_ptr<Icon> _LockIcon;
		std::shared_ptr<Icon> _EyeIcon;
		std::shared_ptr<Icon> _LayerRenameIcon;

		GameObject* _DraggingSelectedGameObject;

		bool _EditLayerNameMode;
	};
}