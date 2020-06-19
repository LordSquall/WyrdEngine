project "json"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
	srcdir = "../Osiris/vendor/json/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        srcdir .. "include/nlohmann/**.hpp",
    }
    
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"