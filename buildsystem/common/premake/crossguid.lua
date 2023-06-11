project "crossguid"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"

	project_dir_name = "crossguid"
	project_dependices_root = "../../../dependencies/"

	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"
    
	src_dir = project_dependices_root .. project_dir_name .. "/src/"
	include_dir = project_dependices_root .. project_dir_name .. "/include/"
	
	basedir(base_dir)
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        include_dir .. "**.hpp",
        src_dir .. "**.cpp",
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
		