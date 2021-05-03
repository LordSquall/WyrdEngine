externallibsdir = os.getenv("WYRD_EXTERN_REPOS")

project "spdlog"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	srcdir = externallibsdir .. "/spdlog/src/"
	include_dir = externallibsdir .. "/spdlog/include"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        srcdir .. "**.h",
		srcdir .. "**.cpp"
    }
	
	includedirs	
	{
		include_dir
	}
	
    
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SPDLOG_COMPILED_LIB"
		}
	
	filter "system:linux"
		systemversion "latest"

		defines
		{
			"SPDLOG_COMPILED_LIB"
		}
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"