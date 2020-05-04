workspace "Osiris"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}"

includedir = {}
includedir["GLFW"] = "Osiris/vendor/GLFW/include"
includedir["GLAD"] = "Osiris/vendor/GLAD/include"
includedir["SOIL"] = "Osiris/vendor/soil/src"
includedir["glm"] = "Osiris/vendor/glm"
includedir["imgui"] = "Osiris/vendor/imgui"
includedir["json"] = "Osiris/vendor/json/include"
includedir["tinyobjloader"] = "Osiris/vendor/tinyobjloader/include"

include "Osiris/vendor/GLFW"
include "Osiris/vendor/GLAD"
include "Osiris/vendor/SOIL"
include "Osiris/vendor/imgui"
include "Osiris/vendor/json"

project "Osiris"
	location "Osiris"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")


	pchheader "osrpch.h"
	pchsource "Osiris/src/osrpch.cpp"

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
		"%{prj.name}/vendor/spdlog/include",
		"%{includedir.GLFW}",
		"%{includedir.GLAD}",
		"%{includedir.SOIL}",
		"%{includedir.json}",
		"%{includedir.glm}",
		"%{includedir.tinyobjloader}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"SOIL",
		"opengl32.dll"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OSR_PLATFORM_WINDOWS",
			"OSR_EDITOR_ENABLED",
			"GLFW_INCLUDE_NONE",
			"GLM_ENABLE_EXPERIMENTAL"
		}

	filter "configurations:Debug"
		defines "ORS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ORS_RELEASE"
		runtime "Release"
		symbols "on"

	filter "configurations:Distribution"
		defines "ORS_DISTRIBUTION"
		runtime "Release"
		symbols "on"

project "Editor"
			location "Apps/Editor"
			kind "ConsoleApp"
			language "C++"
			cppdialect "C++17"
			staticruntime "on"

			targetdir ("bin/" .. outputdir .. "/%{prj.name}")
			objdir ("obj/" .. outputdir .. "/%{prj.name}")

			files
			{
				"Apps/%{prj.name}/src/**.h",
				"Apps/%{prj.name}/src/**.cpp",
				"Apps/%{prj.name}/res/**.vs",
				"Apps/%{prj.name}/res/**.fs"
			}

			includedirs
			{
				"Apps/%{prj.name}/src",
				"Osiris/src",
				"%{includedir.glm}",
				"Osiris/vendor/spdlog/include",
				"%{includedir.GLFW}",
				"%{includedir.GLAD}",
				"%{includedir.SOIL}",
				"%{includedir.imgui}",
				"%{includedir.glm}",
				"%{includedir.json}",
				"%{includedir.tinyobjloader}"
			}

			links
			{
				"Osiris",
				"GLFW",
				"GLAD",
				"SOIL",
				"imgui",
				"opengl32.dll"
			}

			filter "system:windows"
				systemversion "latest"

				defines
				{
					"OSR_PLATFORM_WINDOWS",
					"OSR_EDITOR_ENABLED",
					"GLM_ENABLE_EXPERIMENTAL"
				}

			filter "configurations:Debug"
				defines "ORS_DEBUG"
				runtime "Debug"
				symbols "on"

			filter "configurations:Release"
				defines "ORS_RELEASE"
				runtime "Debug"
				symbols "on"

			filter "configurations:Distribution"
				defines "ORS_DISTRIBUTION"
				runtime "Debug"
				symbols "on"


project "Experimental"
			location "Apps/Experimental"
			kind "ConsoleApp"
			language "C++"
			cppdialect "C++17"
			staticruntime "on"

			targetdir ("bin/" .. outputdir .. "/%{prj.name}")
			objdir ("obj/" .. outputdir .. "/%{prj.name}")

			files
			{
				"Apps/%{prj.name}/src/**.h",
				"Apps/%{prj.name}/src/**.cpp",
				"Apps/%{prj.name}/res/**.vs",
				"Apps/%{prj.name}/res/**.fs"
			}

			includedirs
			{
				"Apps/%{prj.name}/src",
				"Osiris/src",
				"%{includedir.glm}",
				"Osiris/vendor/spdlog/include",
				"%{includedir.GLFW}",
				"%{includedir.GLAD}",
				"%{includedir.SOIL}",
				"%{includedir.imgui}",
				"%{includedir.glm}",
				"%{includedir.json}",
				"%{includedir.tinyobjloader}"
			}

			links
			{
				"Osiris",
				"GLFW",
				"GLAD",
				"SOIL",
				"imgui",
				"opengl32.dll"
			}

			filter "system:windows"
				systemversion "latest"

				defines
				{
					"OSR_PLATFORM_WINDOWS",
					"OSR_EDITOR_ENABLED",
					"GLM_ENABLE_EXPERIMENTAL"
				}

			filter "configurations:Debug"
				defines "ORS_DEBUG"
				runtime "Debug"
				symbols "on"

			filter "configurations:Release"
				defines "ORS_RELEASE"
				runtime "Debug"
				symbols "on"

			filter "configurations:Distribution"
				defines "ORS_DISTRIBUTION"
				runtime "Debug"
				symbols "on"
