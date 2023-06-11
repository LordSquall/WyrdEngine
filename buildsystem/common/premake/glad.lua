project "glad"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	project_dir_name = "glad"
	project_dependices_root = "../../../dependencies/"
    
	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"

	src_dir = "../../../Wyrd/vendor/" .. project_dir_name .. "/"
	include_dir = "../../../Wyrd/vendor/" .. project_dir_name .. "/include/"
	
	basedir(project_dependices_root .. project_dir_name .. "/premakedir/")
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        src_dir .. "**.h",
		src_dir .. "**.c"
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