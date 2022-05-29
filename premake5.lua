workspace "Wyrd"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

-- On windows, check for the installation of mono
monofound = false
monolibdir = ""
monodir = ""

if os.istarget("windows") then
	if os.isfile("C:/PROGRA~1/Mono/etc/mono/config") == true then
		print("Mono installation found")
		monofound = true
		monolibdir = "C:/Program Files/Mono/lib/"
		monobindir = "C:/Program Files/Mono/bin/"
		monodir = "C:/PROGRA~1/Mono/"
	else
		print("Unable to find Mono installation. Please install before continuing")
		os.exit()
	end
elseif os.istarget("linux") then
	if os.isfile("/bin/mono") == true then
		print("Mono installation found")
		monofound = true
		monolibdir = "/usr/lib/"
		monodir = "/usr/"
	else
		print("Unable to find Mono installation. Please install before continuing")
		os.exit()
	end
end


dependenciesdir = "dependencies/"

outputdir = "%{cfg.buildcfg}"

includedir = {}
includedir["GLFW"] = dependenciesdir .. "/GLFW/include"
includedir["SOIL"] = dependenciesdir .. "/soil/src"
includedir["glm"] = dependenciesdir .. "/glm"
includedir["imgui"] = dependenciesdir .. "/imgui"
includedir["spdlog"] = dependenciesdir .. "/spdlog/include/"
includedir["uuid"] = dependenciesdir .. "/uuid/include/"
includedir["hash"] = dependenciesdir .. "/Hash/include/"
includedir["freetype"] = dependenciesdir .. "/freetype/include/"
includedir["jsonxx"] = dependenciesdir .. "/jsonxx/"
includedir["GLAD"] = "Wyrd/vendor/glad/include"

-- if mono was found, the add the in application to the include directories
if monofound then
	includedir["mono"] = monodir .. "include/mono-2.0/"
end

if os.istarget("windows") then
group "Third Party"
	include "buildsystem/windows/premake5-glfw.lua"
	include "buildsystem/windows/premake5-glad.lua"
	include "buildsystem/windows/premake5-jsonxx.lua"
	include "buildsystem/windows/premake5-soil.lua"
	include "buildsystem/windows/premake5-imgui.lua"
	include "buildsystem/windows/premake5-spdlog.lua"
	include "buildsystem/windows/premake5-uuid.lua"
	include "buildsystem/windows/premake5-hash.lua"
	include "buildsystem/windows/premake5-freetype.lua"
end

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
		
		libdirs
		{
			monolibdir,
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
			
			includedirs
			{
				"%{prj.name}/src/",
				"%{includedir.GLFW}",
				"%{includedir.GLAD}",
				"%{includedir.jsonxx}",
				"%{includedir.SOIL}",
				"%{includedir.glm}",
				"%{includedir.mono}",
				"%{includedir.spdlog}",
				"%{includedir.uuid}"
			}
			
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
				"WYRD_PLATFORM_LINUX",
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
			
group "Applications"
project "TestPlayer"
	location "Apps/TestPlayer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"Apps/%{prj.name}/src/",
		"%{prj.name}/vendor/glad/include",
		"Wyrd/src",
		"%{includedir.GLFW}",
		"%{includedir.GLAD}",
		"%{includedir.jsonxx}",
		"%{includedir.SOIL}",
		"%{includedir.glm}",
		"%{includedir.mono}",
		"%{includedir.spdlog}",
		"%{includedir.uuid}"
	}
	
	libdirs
	{
		externallibsdir .. "/freetype/builds/Debug/"
	}

	links
	{
		"Wyrd",
		"GLFW",
		"GLAD",
		"SOIL",
		"jsonxx",
		"imgui",
		"freetyped",
		"opengl32.dll"
	}
	
	dependson 
	{ 
		"WyrdAPI", 
		"WyrdCAPI"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WYRD_PLATFORM_WINDOWS",
			"WYRD_EDITOR_ENABLED",
			"GLM_ENABLE_EXPERIMENTAL",
			"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
			"MONO_INSTALL_LOC=" .. monodir
		}
		
		linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
		debugenvs { "PATH=" .. monobindir .. ";../../lib/Debug/WyrdCAPI/;%PATH%" }
		

	filter "configurations:Debug"
		defines "WYRD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "WYRD_RELEASE"
		runtime "Debug"
		symbols "on"

	filter "configurations:Distribution"
		defines "WYRD_DISTRIBUTION"
		runtime "Debug"
		symbols "on"
		
project "Editor"
	location "Apps/Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"Apps/Players/Player/src",
		"codegen/src/Common/include",
		"codegen/src/PlayerAPI/include",
		"Wyrd/src",
		"%{includedir.glm}",
		"%{includedir.GLFW}",
		"%{includedir.GLAD}",
		"%{includedir.jsonxx}",
		"%{includedir.SOIL}",
		"%{includedir.imgui}",
		"%{includedir.glm}",
		"%{includedir.tinyobjloader}",
		"%{includedir.mono}",
		"%{includedir.spdlog}",
		"%{includedir.uuid}",
		"%{includedir.hash}",
		"%{includedir.freetype}"
	}
	
	libdirs
	{
		externallibsdir .. "/freetype/builds/Debug/"
	}

	links
	{
		"Wyrd",
		"GLFW",
		"GLAD",
		"SOIL",
		"jsonxx",
		"imgui",
		"freetyped",
		"opengl32.dll"
	}
	
	dependson 
	{ 
		"WyrdAPI", 
		"WyrdCAPI",
		"Player"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WYRD_PLATFORM_WINDOWS",
			"WYRD_EDITOR_ENABLED",
			"GLM_ENABLE_EXPERIMENTAL",
			"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
			"MONO_INSTALL_LOC=" .. monodir

		}
		
		linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
		debugenvs { "PATH=" .. monobindir .. ";../../lib/Debug/WyrdCAPI/;%PATH%" }
		
		

	filter "configurations:Debug"
		defines "WYRD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "WYRD_RELEASE"
		runtime "Debug"
		symbols "on"

	filter "configurations:Distribution"
		defines "WYRD_DISTRIBUTION"
		runtime "Debug"
		symbols "on"