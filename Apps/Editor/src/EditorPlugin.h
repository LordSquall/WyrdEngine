#pragma once

#include "osrpch.h"

#include "core/export.h"

#include "support/IconLibrary.h"

namespace Osiris::Editor
{
	class EditorPlugin
	{
	public:
		EditorPlugin(const std::string& name = "ImGuiPlugin") { m_Name = name; }
		virtual ~EditorPlugin() { }

		void SetIconLibrary(IconLibrary* iconLibrary) { m_IconLibrary = iconLibrary; }

		virtual void OnInitialise() {};
		virtual void OnEditorRender() {};

		inline const std::string& GetName() const { return m_Name; }


		inline bool* GetShowFlagRef() { return &m_ShowFlag; }
	protected:
		std::string m_Name;

		bool m_ShowFlag = true;

		IconLibrary* m_IconLibrary;
	};
}