#include "core/export.h"

#include "EditorPlugin.h"
#include "services/ServiceManager.h"

#include "datamodels/logging/LogMessage.h"

namespace Osiris::Editor
{
	class OutputView : public EditorPlugin
	{
	public:
		OutputView();
		~OutputView();

		void OnEditorRender() override;

	private:
		void DrawInformationLogItem(const LogMessage& msg);
		void DrawScriptLogItem(const LogMessage& msg);

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<ResourceService> _ResourceService;

		bool _ShowInfo = true;
		bool _ShowWarnings = true;
		bool _ShowErrors = true;
		bool _ShowDebug = true;

		std::vector<std::shared_ptr<const LogMessage>> _LogItems;
	};
}