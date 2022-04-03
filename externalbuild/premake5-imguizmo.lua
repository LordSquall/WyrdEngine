externallibsdir = os.getenv("WYRD_EXTERN_REPOS")

project "imguizmo"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	srcdir = externallibsdir .. "/imguizmo/"
	include_dir = externallibsdir .. "/imguizmo/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	dependson 
	{ 
		"imgui"
	}
	
	files
	{
        --srcdir .. "GraphEditor.h",
		srcdir .. "ImCurveEdit.h",
		srcdir .. "ImGradient.h",
		srcdir .. "ImGuizmo.h",
		srcdir .. "ImSequencer.h",
		srcdir .. "ImZoomSlider.h",
		--srcdir .. "GraphEditor.cpp",
		srcdir .. "ImCurveEdit.cpp",
		srcdir .. "ImGradient.cpp",
		srcdir .. "ImGuizmo.cpp",
		srcdir .. "ImSequencer.cpp"
    }
	
	includedirs
	{
		include_dir,
		externallibsdir .. "/imgui/"
	}
    
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"