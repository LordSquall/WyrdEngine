
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

		void DrawItemTable(const std::vector<std::filesystem::path>& directories, const std::vector<std::filesystem::path>& files);
		void DrawFolderItem(int resID, const std::filesystem::path& directoryEntry);
		void DrawTextureItem(int resID, TextureRes& textureResource);
		void DrawSceneItem(int resID, SceneRes& sceneResource);
		void DrawScriptItem(int resID, ScriptRes& scriptResource);
		void DrawMaterialItem(int resID, MaterialRes& materialResource);
		void DrawModelItem(int resID, ModelRes& modelResource);
		void DrawShaderItem(int resID, ShaderRes& ShaderResource);
		void DrawUnknownItem(int resID, const std::string& unknownResourceName);

		/* Selection State(s) */
		std::filesystem::path _SelectedDirectory;
		int _SelectedResource;
		std::string _DeleteDirectoryState;
		std::string _DeleteAssetState;

		/* Services*/
		std::shared_ptr<ResourceService>	_Resources;
		std::shared_ptr<WorkspaceService>	_Workspace;
		std::shared_ptr<SettingsService>	_Settings;
		std::shared_ptr<EventService>		_Events;
		std::shared_ptr<DialogService>		_Dialogs;

		std::string _SearchCriteria;
	};
}