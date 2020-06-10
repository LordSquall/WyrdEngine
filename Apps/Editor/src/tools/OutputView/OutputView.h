#include "core/export.h"

#include "EditorPlugin.h"
#include "services/ServiceManager.h"

#include "datamodels/logging/LogMessage.h"

namespace Osiris::Editor
{
	class OutputView : public EditorPlugin
	{
		struct LogItem
		{
			Severity severity;
			const std::string message;
		};

	public:
		OutputView();
		~OutputView();

		void OnEditorRender() override;

	private:
		void DrawLogItem(const LogItem& msg);

	private:
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<ResourceService> _ResourceService;

		bool _ShowInfo = true;
		bool _ShowWarnings = true;
		bool _ShowErrors = true;
		bool _ShowDebug = true;

		std::vector<LogItem> _LogItems;
	};
}