workspace "Wyrd"
	architecture "x64"
	startproject "Wyrd"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

externallibsdir = os.getenv("WYRD_EXTERN_REPOS")

renderdocfound = false
renderdocdir = ""

monofound = false
monolibdir = ""
monodir = ""

if os.istarget("windows") then
	-- check for the installation of the renderdoc api header file
	if os.isfile("C:/PROGRA~1/RenderDoc/renderdoc_app.h") == true then
		print("Render Doc header found")
		renderdocfound = true
		renderdocdir = "C:/Program Files/RenderDoc/"
	end
end

-- check for the installation of the mono file
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

outputdir = "%{cfg.buildcfg}"

includedir = {}
includedir["GLFW"] = externallibsdir .. "/GLFW/include"
includedir["GLAD"] = "Wyrd/vendor/GLAD/include"
includedir["glm"] = externallibsdir .. "/glm"
includedir["spdlog"] = externallibsdir .. "/spdlog/include/"
includedir["uuid"] = externallibsdir .. "/crossguid/include/"
includedir["freetype"] = externallibsdir .. "/freetype/include/"

-- if mono was found, the add the in application to the include directories
if monofound then
	includedir["mono"] = monodir .. "include/mono-2.0/"
end

-- if renderdoc was found, the add the in application to the include directories
if renderdocfound then
	includedir["renderdoc"] = renderdocdir
end

group "Third Party"
include "externalbuild/premake5-glfw.lua"
include "externalbuild/premake5-glad.lua"
include "externalbuild/premake5-spdlog.lua"
include "externalbuild/premake5-uuid.lua"

group ""
	project "Wyrd"
		location "Wyrd"
		kind "SharedLib"
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
			"%{includedir.GLFW}",
			"%{includedir.GLAD}",
			"%{includedir.glm}",
			"%{includedir.tinyobjloader}",
			"%{includedir.mono}",
			"%{includedir.spdlog}",
			"%{includedir.uuid}"
		}
		
		libdirs
		{
			monolibdir,
		}
		
		links
		{
			"GLFW",
			"GLAD",
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
				"WYRD_SHARED_LIB",
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


group "Scripting"			
project "WyrdCAPI"
		location "WyrdCAPI"
		kind "SharedLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir ("lib/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/**.cpp",
			"%{prj.name}/**.h"
		}
		
		includedirs
		{
			"Wyrd/src",
			"%{includedir.glm}",
			"%{includedir.uuid}",
			"%{includedir.mono}"
		}
		
        links { 
			"uuid",
			"Wyrd"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_EDITOR_ENABLED",
				"GLFW_INCLUDE_NONE",
				"GLM_ENABLE_EXPERIMENTAL",
				"LIBRARY_EXPORT"
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
			
project "WyrdAPI"
		location "WyrdAPI"
		kind "SharedLib"
		language "C#"

		targetdir ("lib/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/**.cs"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_EDITOR_ENABLED",
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