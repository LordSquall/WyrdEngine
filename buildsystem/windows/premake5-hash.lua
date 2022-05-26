externallibsdir = os.getenv("WYRD_EXTERN_REPOS")

project "hash"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	include_dir = externallibsdir .. "/Hash/src/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        include_dir .. "**.h"
    }
	
	includedirs	
	{
		include_dir
	}
	
    
	filter "system:windows"
		systemversion "latest"
	
	filter "system:linux"
		systemversion "latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"
		