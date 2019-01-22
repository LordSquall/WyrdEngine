workspace "Osiris"
	architecture "x64"
	startproject "EditorApp"
		
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
	kind "SharedLib"
	language "C++"
	
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
		"%{includedir.imgui}",
		"%{includedir.json}",
		"%{includedir.tinyobjloader}"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"SOIL",
		"imgui",
		"opengl32.dll"
	}
		
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/DevApp")
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		characterset "MBCS"
		
		defines
		{
			"OSR_PLATFORM_WINDOWS",
			"OSR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
	filter "configurations:Debug"
		defines "ORS_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "ORS_RELEASE"
		buildoptions "/MD"
		symbols "On"
	
	filter "configurations:Distribution"
		defines "ORS_DISTRIBUTION"
		buildoptions "/MD"
		symbols "On"	

project "OsirisEditor"
	location "OsirisEditor"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/vendor/spdlog/include",
		"Osiris/src",
		"Osiris/vendor/spdlog/include",
		"%{includedir.GLFW}",
		"%{includedir.GLAD}",
		"%{includedir.SOIL}",
		"%{includedir.imgui}",
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
		
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/DevApp")
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		characterset "MBCS"
		
		defines
		{
			"OSR_PLATFORM_WINDOWS",
			"OSR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
	filter "configurations:Debug"
		defines "ORS_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "ORS_RELEASE"
		buildoptions "/MD"
		symbols "On"
	
	filter "configurations:Distribution"
		defines "ORS_DISTRIBUTION"
		buildoptions "/MD"
		symbols "On"				
		
project "DevApp"
	location "Apps/DevApp"
	kind "ConsoleApp"
	language "C++"
	
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
		"%{prj.name}/src/",
		"Osiris/src",
		"Osiris/vendor/spdlog/include"
	}
	
	links
	{
		"Osiris"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		characterset "MBCS"
		
		defines
		{
			"OSR_PLATFORM_WINDOWS"
		}
	
		filter "configurations:Debug"
		defines "ORS_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "ORS_RELEASE"
		buildoptions "/MD"
		symbols "On"
	
	filter "configurations:Distribution"
		defines "ORS_DISTRIBUTION"
		buildoptions "/MD"
		symbols "On"	
