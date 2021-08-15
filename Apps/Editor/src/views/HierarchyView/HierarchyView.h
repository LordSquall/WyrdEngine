#include "core/export.h"

#include "services/ServiceManager.h"
#include "views/EditorViewBase.h"
#include "events/KeyEvent.h"

namespace Wyrd::Editor
{
	class HierarchyView : public EditorViewBase
	{
	public:
		HierarchyView(EditorLayer* editorLayer);
		~HierarchyView();

		void OnInitialise() override;
		void OnEditorRender() override;

		void OnEvent(Event& event) override;

	private:
		void DisplayEntity(Entity entity);
		void DisplayEntityContentMenu(Entity entity);
		void DisplayEntityDragAndDrop(Entity entity);
		void DisplayEntityTooltip(Entity entity);
		void OnSelectedEntityChanged(Events::EventArgs& args);

		bool OnKeyPressedEvent(KeyPressedEvent& e);

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		Entity _SelectedEntity;
	};
}