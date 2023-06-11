project "soil"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	project_dir_name = "soil"
	project_dependices_root = "../../../dependencies/"

	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"

	src_dir = project_dependices_root .. project_dir_name .. "/"
	
	basedir(project_dependices_root .. project_dir_name .. "/premakedir/")
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        src_dir .. "**.h",
		src_dir .. "**.c"
    }
	
	excludes 
	{ 
		src_dir .. "src/test_SOIL.cpp",
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