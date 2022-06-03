project "spdlog"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	project_dir_name = "spdlog"
	project_dependices_root = "../../../dependencies/"

	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"

	src_dir = project_dependices_root .. project_dir_name .. "/src/"
	include_dir = project_dependices_root .. project_dir_name .. "/include"
	
	basedir(project_dependices_root .. project_dir_name .. "/premakedir/")
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        src_dir .. "**.h",
		src_dir .. "**.cpp"
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