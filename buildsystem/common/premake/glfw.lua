print("GLFW Premake build started")

project "glfw"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	project_dir_name = "glfw"
	project_dependices_root = "../../../dependencies/"

	base_dir = project_dependices_root .. project_dir_name .. "/premakedir/"

	src_dir = project_dependices_root .. project_dir_name .. "/"
	
	basedir(project_dependices_root .. project_dir_name .. "/premakedir/")
	
	targetdir (base_dir .. "bin/" .. outputdir .. "/" .. project_dir_name)
    objdir (base_dir .. "bin-int/" .. outputdir .. "/" .. project_dir_name)

	files
	{
        src_dir .. "include/GLFW/glfw3.h",
        src_dir .. "include/GLFW/glfw3native.h",
        src_dir .. "src/glfw_config.h",
        src_dir .. "src/context.c",
        src_dir .. "src/platform.c",
        src_dir .. "src/init.c",
        src_dir .. "src/input.c",
        src_dir .. "src/monitor.c",
        src_dir .. "src/vulkan.c",
        src_dir .. "src/window.c",
        src_dir .. "src/null_init.c",
        src_dir .. "src/null_monitor.c",
        src_dir .. "src/null_joystick.c",
        src_dir .. "src/null_window.c"
    }
    
	filter "system:linux"
		systemversion "latest"
		
		files
        {
            src_dir .. "src/x11_init.c",
            src_dir .. "src/linux_joystick.c",
			src_dir ..  "src/x11_monitor.c",
            src_dir .. "src/posix_time.c",
            src_dir .. "src/posix_thread.c",
            src_dir .. "src/x11_window.c",
            src_dir .. "src/glx_context.c",
            src_dir .. "src/egl_context.c",
            src_dir .. "src/osmesa_context.c"
        }

        defines 
		{ 
            "_GLFW_X11"
		}
        
    filter "system:windows"
		systemversion "latest"
		
		files
        {
            src_dir .. "src/win32_init.c",
            src_dir .. "src/win32_joystick.c",
			src_dir ..  "src/win32_monitor.c",
            src_dir .. "src/win32_time.c",
            src_dir .. "src/win32_thread.c",
            src_dir .. "src/win32_window.c",
            src_dir .. "src/win32_module.c",
            src_dir .. "src/wgl_context.c",
            src_dir .. "src/egl_context.c",
            src_dir .. "src/osmesa_context.c"
        }

		defines 
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"