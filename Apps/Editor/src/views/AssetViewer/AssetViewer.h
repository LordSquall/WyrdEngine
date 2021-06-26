
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

		bool DirectoryTreeViewRecursive(const std::string& path);
		void DrawDirectoryContextMenu(const std::string& path);
		bool DrawEditDirectoryNode();

		void DrawAssetsItems();

		void DrawTextureItem(uint32_t resIdx, TextureRes& textureResource);
		void DrawSceneItem(uint32_t resIdx, SceneRes& sceneResource);
		void DrawScriptItem(uint32_t resIdx, ScriptRes& scriptResource);
		void DrawUnknownItem(uint32_t resIdx, std::string& unknownResourceName);

		/* Selection State(s) */
		std::string _SelectedDirectory;
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
		std::shared_ptr<Icon> _SceneIcon;
		std::shared_ptr<Icon> _TextureIcon;
		std::shared_ptr<Icon> _ScriptIcon;
	};
}