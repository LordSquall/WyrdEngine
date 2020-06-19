project "lua"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
	src_dir = "../Osiris/vendor/lua/"
	include_dir = "../Osiris/vendor/lua/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        src_dir .. "**.h",
		src_dir .. "**.c"
    }
	
	includedirs
	{
		include_dir
	}
	
	defines
	{
		"MAKE_LIB"
	}
    
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"