#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"

namespace Osiris::Editor
{
	class OSIRIS_API SceneHierarchy : public EditorPlugin
	{
	public:
		SceneHierarchy();
		~SceneHierarchy();
		
		void OnEditorRender() override;

	};
}