#pragma once

#include "core/export.h"

#include "editor/services/IService.h"
#include "editor/datamodels/Scene.h"

namespace Osiris::Editor
{
	enum Dialogs
	{
		CreateNewProject = 0
	};

	class DialogService : public IService
	{
	public:
		DialogService() {}
		~DialogService() {}

		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnGUI() override;

		void OpenDialog(Dialogs dialog);

	//private:
		bool _isDialogOpen;
	};
}