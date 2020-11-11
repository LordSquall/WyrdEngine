externallibsdir = os.getenv("OSIRIS_EXTERN_REPOS")

print("GLFW Premake build started")

project "glfw"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
	srcdir = externallibsdir .. "/glfw/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        srcdir .. "include/GLFW/glfw3.h",
        srcdir .. "include/GLFW/glfw3native.h",
        srcdir .. "src/glfw_config.h",
        srcdir .. "src/context.c",
        srcdir .. "src/init.c",
        srcdir .. "src/input.c",
        srcdir .. "src/monitor.c",
        srcdir .. "src/vulkan.c",
        srcdir .. "src/window.c"
    }
    
	filter "system:linux"
		systemversion "latest"
		
		files
        {
            srcdir .. "src/x11_init.c",
            srcdir .. "src/linux_joystick.c",
			srcdir ..  "src/x11_monitor.c",
            srcdir .. "src/posix_time.c",
            srcdir .. "src/posix_thread.c",
            srcdir .. "src/x11_window.c",
            srcdir .. "src/glx_context.c",
            srcdir .. "src/egl_context.c",
            srcdir .. "src/osmesa_context.c"
        }

        defines 
		{ 
            "_GLFW_X11"
		}
        
    filter "system:windows"
		systemversion "latest"
		
		files
        {
            srcdir .. "src/win32_init.c",
            srcdir .. "src/win32_joystick.c",
			srcdir ..  "src/win32_monitor.c",
            srcdir .. "src/win32_time.c",
            srcdir .. "src/win32_thread.c",
            srcdir .. "src/win32_window.c",
            srcdir .. "src/wgl_context.c",
            srcdir .. "src/egl_context.c",
            srcdir .. "src/osmesa_context.c"
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