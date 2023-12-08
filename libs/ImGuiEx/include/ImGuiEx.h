#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <core/scene/Scene.h>
#include <core/ecs/ECS.h>

#ifdef WYRD_PLATFORM_LINUX
typedef long INT_PTR;
typedef unsigned long UINT_PTR;
#endif

namespace ImGui
{
	const static ImVec4 BG_COLOR = ImVec4(0, 0, 0, 0);
	const static ImVec4 TINT_COLOR = ImVec4(1, 1, 1, 1);
	
	const static std::string XCOMP_LABEL = "x";
	const static std::string YCOMP_LABEL = "y";

	/* Tree View  */
	bool TreeEntity(Wyrd::Scene& scene, Wyrd::Entity e);
	void TreeSeperator(const int depth);
}