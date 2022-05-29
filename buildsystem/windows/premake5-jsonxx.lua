project "jsonxx"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	src_dir = "../../dependencies/jsonxx/"
	include_dir = "../../dependencies/jsonxx/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        src_dir .. "**.h",
		src_dir .. "**.cc"
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