workspace "Wyrd"
	architecture "x64"
	startproject "ESCTestApp"

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