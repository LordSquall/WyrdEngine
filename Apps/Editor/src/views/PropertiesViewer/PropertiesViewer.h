/* core osiris includes */
#include <core/export.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "IPropertyView.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "datamodels/resources/Resource.h"

namespace Osiris::Editor
{
	class EventService;

	class PropertiesViewer : public EditorViewBase
	{
	public:
		enum Mode {
			None = 0, GameObjectUI = 1, AssetUI = 2
		};

	public:
		PropertiesViewer(EditorLayer* editorLayer);
		~PropertiesViewer();

		/* EditorPlugin functions */
		void OnEditorRender() override;

	private:
		Mode _Mode;
		void OnSelectedGameObjectChanged(Events::EventArgs& args);
		void OnSelectedAssetChanged(Events::EventArgs& args);
		
		void RefreshComponentViews();
		
		void DrawGameObjectUI();
		void DrawAssetUI();

		std::shared_ptr<EventService> _EventService;
		std::vector<std::unique_ptr<IPropertiesView>> _PropertiesViews;
		static Osiris::GameObject* _SelectedGameObject;
		static std::shared_ptr<Resource> _SelectedAsset;
	};
}