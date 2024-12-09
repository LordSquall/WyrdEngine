project "imgui"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	project_dir_name = "imgui"
	project_dependices_root = "../../../dependencies/"

	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"

	src_dir = project_dependices_root .. project_dir_name .. "/"
	include_dir = project_dependices_root .. project_dir_name .. "/"
	
	basedir(project_dependices_root .. project_dir_name .. "/premakedir/")
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        src_dir .. "imconfig.h",
		src_dir .. "imgui.h",
		src_dir .. "imgui_internal.h",
		src_dir .. "imstb_rectpack.h",
		src_dir .. "imstb_textedit.h",
		src_dir .. "imstb_truetype.h",
		src_dir .. "misc/cpp/imgui_stdlib.h",
        src_dir .. "backends/imgui_impl_glfw.h",
		src_dir .. "imgui.cpp",
		src_dir .. "imgui_demo.cpp",
		src_dir .. "imgui_tables.cpp",
		src_dir .. "imgui_draw.cpp",
		src_dir .. "imgui_widgets.cpp",
		src_dir .. "misc/cpp/imgui_stdlib.cpp",
        src_dir .. "backends/imgui_impl_glfw.cpp",
    }
	
	includedirs
	{
		include_dir,
		project_dependices_root .. "/GLFW/include"
	}

	links
	{
		"GLFW"
	}
    
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"