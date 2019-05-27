#include "core/export.h"
#include "core/Layer.h"

#include "editor/EditorPlugin.h"
#include "editor/scene/Scene.h"

namespace Osiris::Editor
{
	class SceneEditor : public EditorPlugin
	{
	public:
		SceneEditor();
		~SceneEditor();
		
		void OnEditorRender() override;

	private:
		void PopulateSceneTree(Node* rootNode);

	private:
		Scene m_Scene;
	};
}