#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"

namespace Osiris::Editor
{
	class SceneHierarchy : public EditorPlugin
	{
	public:
		SceneHierarchy();
		~SceneHierarchy();
		
		void OnEditorRender() override;

	};
}