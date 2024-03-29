#include "core/export.h"

#include "views/EditorViewBase.h"
#include "services/ServiceManager.h"

#include "datamodels/logging/LogMessage.h"

namespace Wyrd::Editor
{
	class OutputView : public EditorViewBase
	{
		struct LogItem
		{
			LogType type;
			Severity severity;
			std::string message;
		};

	public:
		OutputView(EditorLayer* editorLayer);
		~OutputView();

		void OnEditorRender() override;

	private:
		void DrawCoreLogItem(int idx, const LogItem& item);
		void DrawCodeLogItem(int idx, const LogItem& item);
		void DrawUserLogItem(int idx, const LogItem& item);

	private:
		std::shared_ptr<EventService> _Events;
		std::shared_ptr<ResourceService> _Resources;

		bool _ShowInfo = true;
		bool _ShowWarnings = true;
		bool _ShowErrors = true;
		bool _ShowDebug = true;

		std::vector<LogItem> _LogItems;
	};
}