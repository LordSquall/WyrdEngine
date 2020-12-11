#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "views/EditorViewDialogBase.h"

namespace Osiris::Editor
{
	class DialogService : public IService
	{
	public:
		DialogService() {}
		~DialogService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnGUI() override;

		void OpenDialog(std::shared_ptr<EditorViewDialogBase> dialog);

		static std::shared_ptr<EditorViewDialogBase> _activeDialog;
		static bool(*_popupDialogCallback)(std::shared_ptr<EditorViewDialogBase>);
	};
}