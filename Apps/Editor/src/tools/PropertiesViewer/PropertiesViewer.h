#include "core/export.h"
#include "EditorPlugin.h"

#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class PropertiesViewer : public EditorPlugin
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

		static std::shared_ptr<GameObject> _SelectedGameObject;
		static std::shared_ptr<Resource> _SelectedAsset;
	};
}