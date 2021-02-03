externallibsdir = os.getenv("OSIRIS_EXTERN_REPOS")

project "imgui"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	srcdir = externallibsdir .. "/imgui/"
	include_dir = externallibsdir .. "/imgui/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        srcdir .. "imconfig.h",
		srcdir .. "imgui.h",
		srcdir .. "imgui_internal.h",
		srcdir .. "imstb_rectpack.h",
		srcdir .. "imstb_textedit.h",
		srcdir .. "imstb_truetype.h",
		srcdir .. "misc/cpp/imgui_stdlib.h",
		srcdir .. "imgui.cpp",
		srcdir .. "imgui_demo.cpp",
		srcdir .. "imgui_draw.cpp",
		srcdir .. "imgui_widgets.cpp",
		srcdir .. "misc/cpp/imgui_stdlib.cpp"
    }
	
	includedirs
	{
		include_dir
	}
    
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"