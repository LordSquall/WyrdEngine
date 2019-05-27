#include "core/export.h"
#include "core/Layer.h"

#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class GameViewer : public EditorPlugin
	{
	public:
		GameViewer();
		~GameViewer();
		
		void OnEditorRender() override;

	};
}