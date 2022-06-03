project "uuid"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	srcdir = "../../crossguid/src/"
	include_dir = "../../crossguid/include/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        include_dir .. "**.hpp",
        srcdir .. "**.cpp",
    }
	
	includedirs	
	{
		include_dir
	}
	
    
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"GUID_WINDOWS"
		}
	
	filter "system:linux"
		systemversion "latest"
		
		defines
		{
			"GUID_LIBUUID"
		}
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"
		