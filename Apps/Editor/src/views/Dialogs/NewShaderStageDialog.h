#include "core/export.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	class EditorLayer;

	class NewShaderStageDialog : public EditorViewDialogBase
	{
	public:
		NewShaderStageDialog(EditorLayer* editorLayer);
		virtual ~NewShaderStageDialog() = default;

		virtual void OnDialogRender();

		inline void SetPrompt(const std::string& prompt) { _prompt = prompt; }
		inline const std::string& GetPrompt(const std::string& prompt) { return _prompt; }


		inline void GetName(const std::string& name) { _name = name; }
		inline const std::string& GetName() { return _name; }

		inline void SetSuccessCallback(std::function<void(std::string, int)> callback) { _successCallback = callback; }
		inline std::function<void(std::string, int)> GetSuccessCallback() { return _successCallback; }

		inline void SetFailureCallback(std::function<void(std::string, int)> callback) { _failureCallback = callback; }
		inline std::function<void(std::string, int)> GetFailureCallback() { return _failureCallback; }

	private:
		std::string _prompt;
		std::string _name;
		int _type;
		std::function<void(std::string, int)> _successCallback;
		std::function<void(std::string, int)> _failureCallback;
	};
}