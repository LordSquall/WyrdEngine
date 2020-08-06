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
	class PropertiesViewer : public EditorViewBase
	{
	public:
		enum Mode {
			None = 0, GameObjectUI = 1, AssetUI = 2
		};

	public:
		PropertiesViewer();
		~PropertiesViewer();

		/* EditorPlugin functions */
		void OnEditorRender() override;

	private:
		Mode _Mode;
		void OnSelectedGameObjectChanged(Events::EventArgs& args);
		void OnSelectedAssetChanged(Events::EventArgs& args);
		
		void DrawGameObjectUI();
		void DrawAssetUI();

		std::list<std::shared_ptr<IPropertiesView>> _PropertiesViews;
		static std::shared_ptr<Osiris::GameObject> _SelectedGameObject;
		static std::shared_ptr<Resource> _SelectedAsset;
	};
}