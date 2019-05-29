#include "core/export.h"
#include "core/Layer.h"

#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class SceneEditor : public EditorPlugin
	{
	public:
		SceneEditor();
		~SceneEditor();
		
		void OnEditorRender() override;
	};
}