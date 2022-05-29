project "freetype"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	srcdir = "../../dependencies/freetype/src/"
	include_dir = "../../dependencies/freetype/include/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        include_dir .. "**.h",
        srcdir .. "**.c",
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
		