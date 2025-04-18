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
	private:
		struct EntityHiearchyNode {
			Entity e;
			std::vector<EntityHiearchyNode*> children;
		};

	public:
		void OnInitialise() override;
		void OnEditorRender() override;

		void OnEvent(Event& event) override;

	private:
		void DisplayEntityNode(const EntityHiearchyNode& entityNode);
		void DisplayEntityNodeContentMenu(const EntityHiearchyNode& entityNode);
		void DisplayEntityNodeDragAndDrop(const EntityHiearchyNode& entityNode);
		void DisplayEntityNodeTooltip(const EntityHiearchyNode& entityNode);
		void DisplayContentMenu();
		void OnSelectedEntityChanged(Events::EventArgs& args);

		bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);

		void DisplayAboveDragTarget(Entity entity, int depth);
		void DisplayBelowDragTarget(Entity entity, int depth);

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		Entity _SelectedEntity;
	};
}