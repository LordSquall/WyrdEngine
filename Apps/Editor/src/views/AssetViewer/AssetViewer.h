
/* core wyrd includes */
#include <core/export.h>
#include <events/KeyEvent.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class AssetViewer : public EditorViewBase
	{
	public:
		AssetViewer(EditorLayer* editorLayer);
		~AssetViewer() = default;

		void OnEditorRender() override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		void DrawTextureItem(int resID, TextureRes& textureResource);
		void DrawSceneItem(int resID, SceneRes& sceneResource);
		void DrawScriptItem(int resID, ScriptRes& scriptResource);
		void DrawUnknownItem(int resID, std::string& unknownResourceName);

		/* Selection State(s) */
		std::filesystem::path _SelectedDirectory;
		Resource* _SelectedResource;
		std::string _DeleteDirectoryState;
		std::string _DeleteAssetState;

		/* Services*/
		std::shared_ptr<ResourceService> _resourcesService;
		std::shared_ptr<WorkspaceService> _workspaceService;
		std::shared_ptr<SettingsService> _settingsService;
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<DialogService> _dialogService;

		/* Icons */
		std::shared_ptr<Icon> _UnknownIcon;
		std::shared_ptr<Icon> _FolderIcon;
		std::shared_ptr<Icon> _SceneIcon;
		std::shared_ptr<Icon> _TextureIcon;
		std::shared_ptr<Icon> _ScriptIcon;

		std::string _SearchCriteria;
	};
}