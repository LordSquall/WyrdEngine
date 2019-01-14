#pragma once

#include "osrpch.h"
#include "core/export.h"

namespace Osiris
{
	class OSIRIS_API ImGuiPlugin
	{
	public:
		ImGuiPlugin(const std::string& name = "ImGuiPlugin") { }
		virtual ~ImGuiPlugin() { }

		virtual void OnEditorRender() {};

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}