workspace "Wyrd"
	architecture "x64"
	startproject "ESCTestApp"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}"


group ""
	project "Wyrd"
		location "Wyrd"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir ("lib/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")

		pchheader "wyrdpch.h"
		pchsource "Wyrd/src/wyrdpch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/res/**.vs",
			"%{prj.name}/res/**.fs"
		}

		includedirs
		{
			"%{prj.name}/src/",
			"%{prj.name}/vendor/glad/include",
			"dependencies/soil/src/"
		}
		
		links
		{
			"GLFW",
			"GLAD",
			"SOIL",
			"jsonxx",
			"uuid",
			"mono-2.0-sgen",
			"opengl32.dll"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_LIBRARY_EXPORT",
				"GLFW_INCLUDE_NONE",
				"GLM_ENABLE_EXPERIMENTAL"
			}

		filter "system:linux"
			systemversion "latest"

			defines
			{
				"WYRD_LINUX",
				"WYRD_LIBRARY_EXPORT",
				"GLFW_INCLUDE_NONE",
				"GLM_ENABLE_EXPERIMENTAL"
			}
			
		filter "configurations:Debug"
			defines "WYRD_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "WYRD_RELEASE"
			runtime "Release"
			symbols "on"

		filter "configurations:Distribution"
			defines "WYRD_DISTRIBUTION"
			runtime "Release"
			symbols "on"