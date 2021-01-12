#include "core/export.h"

#include "views/EditorViewDialogBase.h"

namespace Osiris::Editor
{
	class EditorLayer;

	class ConfirmationDialog : public EditorViewDialogBase
	{
	public:
		ConfirmationDialog(EditorLayer* editorLayer);
		virtual ~ConfirmationDialog() = default;

		virtual void OnDialogRender();

		inline void SetMessage(const std::string& message) { _message = message; }
		inline const std::string& GetMessage(const std::string& message) { return _message; }

		inline void SetSuccessCallback(std::function<void(void*)> callback) { _successCallback = callback; }
		inline std::function<void(void*)> GetSuccessCallback() { return _successCallback; }

		inline void SetFailureCallback(std::function<void(void*)> callback) { _failureCallback = callback; }
		inline std::function<void(void*)> GetFailureCallback() { return _failureCallback; }

		inline void SetDialogData(void* dialogData) { _dialogData = dialogData; }
		inline void* GetDialogData() { return _dialogData; }

	private:
		std::string _message;
		std::function<void(void*)> _successCallback;
		std::function<void(void*)> _failureCallback;
		void* _dialogData;
	};
}