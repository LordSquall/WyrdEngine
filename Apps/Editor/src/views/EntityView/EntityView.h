#include "core/export.h"

#include "services/ServiceManager.h"
#include "views/EditorViewBase.h"
#include "events/KeyEvent.h"

namespace Wyrd::Editor
{
	class EntityView : public EditorViewBase
	{
	public:
		EntityView(EditorLayer* editorLayer);
		~EntityView();

		void OnInitialise() override;
		void OnEditorRender() override;

		void OnEvent(Event& event) override {} 

	private:
		void OnSelectedEntityChanged(Events::EventArgs& args);

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		Entity _SelectedEntity;
	};
}