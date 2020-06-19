project "soil"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
	srcdir = "../Osiris/vendor/soil/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        srcdir .. "**.h",
		srcdir .. "**.c"
    }
	
	excludes 
	{ 
		srcdir .. "src/test_SOIL.cpp",
	}
    
	filter "system:windows"
		systemversion "latest"
		
		links
		{
			"opengl32"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"