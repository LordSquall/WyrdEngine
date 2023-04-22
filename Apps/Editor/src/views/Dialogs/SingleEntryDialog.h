#include "core/export.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	class EditorLayer;

	class SingleEntryDialog : public EditorViewDialogBase
	{
	public:
		SingleEntryDialog(EditorLayer* editorLayer);
		virtual ~SingleEntryDialog() = default;

		virtual void OnDialogRender();

		inline void SetPrompt(const std::string& prompt) { _prompt = prompt; }
		inline const std::string& GetPrompt(const std::string& prompt) { return _prompt; }


		inline void SetContent(const std::string& content) { _content = content; }
		inline const std::string& GetContent(const std::string& content) { return _content; }

		inline void SetSuccessCallback(std::function<void(std::string)> callback) { _successCallback = callback; }
		inline std::function<void(std::string)> GetSuccessCallback() { return _successCallback; }

		inline void SetFailureCallback(std::function<void(std::string)> callback) { _failureCallback = callback; }
		inline std::function<void(std::string)> GetFailureCallback() { return _failureCallback; }

	private:
		std::string _prompt;
		std::string _content;
		std::function<void(std::string)> _successCallback;
		std::function<void(std::string)> _failureCallback;
	};
}