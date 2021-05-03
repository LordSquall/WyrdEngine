workspace "Wyrd"
	architecture "x64"
	startproject "Editor"

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
includedir["SOIL"] = externallibsdir .. "/Simple-OpenGL-Image-Library/src"
includedir["glm"] = externallibsdir .. "/glm"
includedir["jsonxx"] = externallibsdir .. "/jsonxx"
includedir["imgui"] = externallibsdir .. "/imgui"
includedir["efsw"] = externallibsdir .. "/efsw/include/"
includedir["spdlog"] = externallibsdir .. "/spdlog/include/"
includedir["uuid"] = externallibsdir .. "/crossguid/include/"
includedir["hash"] = externallibsdir .. "/Hash/include/"

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
include "externalbuild/premake5-jsonxx.lua"
include "externalbuild/premake5-soil.lua"
include "externalbuild/premake5-imgui.lua"
include "externalbuild/premake5-efsw.lua"
include "externalbuild/premake5-spdlog.lua"
include "externalbuild/premake5-uuid.lua"
include "externalbuild/premake5-hash.lua"

group "Code Gen"
	project "WyrdBindings"
	location "codegen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("lib/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
	files
	{
		"codegen/src/PlayerAPI/include/**.h",
		"codegen/src/PlayerAPI/src/**.cpp",
		"codegen/src/Common/include/**.h",
		"codegen/src/Common/src/**.cpp"
	}
	
	includedirs
	{
		"codegen/src/PlayerAPI/include/",
		"codegen/src/Common/include/",
		"Wyrd/src",
		"%{includedir.uuid}"
	}
	
	links
	{
		"uuid"
	}


	filter "system:windows"
		systemversion "latest"
		
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
			"%{includedir.GLFW}",
			"%{includedir.GLAD}",
			"%{includedir.jsonxx}",
			"%{includedir.SOIL}",
			"%{includedir.glm}",
			"%{includedir.tinyobjloader}",
			"%{includedir.mono}",
			"%{includedir.spdlog}",
			"%{includedir.uuid}"
		}
		
		libdirs
		{
			monolibdir
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
			"%{includedir.mono}",
			"%{includedir.jsonxx}"
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

group "Applications"
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
		"%{includedir.efsw}",
		"%{includedir.spdlog}",
		"%{includedir.uuid}",
		"%{includedir.hash}",
		iif(renderdocfound, includedir["renderdoc"], "")
	}
	
	links
	{
		"Wyrd",
		"WyrdBindings",
		"GLFW",
		"GLAD",
		"SOIL",
		"jsonxx",
		"imgui",
		"efsw",
		"opengl32.dll"
	}
	
	dependson 
	{ 
		"WyrdAPI", 
		"WyrdCAPI",
		"WyrdBindings",
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
			"MONO_INSTALL_LOC=" .. monodir,
			iif(renderdocfound, "WYRD_RENDERDOC_ENABLED", "")

		}
		
		linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
		debugenvs { "PATH=" .. monobindir .. ";" .. renderdocdir .. ";%PATH%" }

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


project "Player"
	location "Apps/Player"
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
		"Wyrd/src",
		"codegen/src/Common/include",
		"codegen/src/PlayerAPI/include",
		"%{includedir.spdlog}",
		"%{includedir.glm}",
		"%{includedir.uuid}",
		"%{includedir.mono}"
	}
	
	links
	{
		"Wyrd",
		"WyrdBindings"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WYRD_PLATFORM_WINDOWS",
			"WYRD_EDITOR_ENABLED",
			"GLM_ENABLE_EXPERIMENTAL",
			"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
			"MONO_INSTALL_LOC=" .. monodir,
			iif(renderdocfound, "WYRD_RENDERDOC_ENABLED", "")

		}
		
		linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
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
