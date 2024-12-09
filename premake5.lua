workspace "Wyrd"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

-- check for the installation of mono
monofound = false
monolibdir = ""
monodir = ""

renderdocfound = false
renderdocdir = ""

if os.istarget("windows") then
	if os.isfile("C:/PROGRA~1/Mono/etc/mono/config") == true then
		print("Mono installation found")
		monofound = true
		monolibdir = "C:/Program Files/Mono/lib/"
		monobindir = "C:/Program Files/Mono/bin/"
		monodir = "C:/PROGRA~1/Mono/"
	else
		print("Unable to find Mono installation. Please install before continuing")
		
	end
	
	if os.isfile("C:/PROGRA~1/RenderDoc/renderdoc_app.h") == true then
		print("Render Doc header found")
		renderdocfound = true
		renderdocdir = "C:/Program Files/RenderDoc/"
	else
		print("Unable to find RenderDoc installation")
	end
elseif os.istarget("linux") then
	if os.isfile("/bin/mono") == true then
		print("Mono installation found")
		monofound = true
		monolibdir = "/usr/lib/"
		monodir = "/usr/"
	else
		print("Unable to find Mono installation. Please install before continuing")
		
	end
end


dependenciesdir = "dependencies/"

outputdir = "%{cfg.buildcfg}"

includedir = {}
-- Third Party libs
includedir["GLFW"] = dependenciesdir .. "/GLFW/include"
includedir["SOIL"] = dependenciesdir .. "/soil/src"
includedir["glm"] = dependenciesdir .. "/glm"
includedir["imgui"] = dependenciesdir .. "/imgui"
includedir["imguizmo"] = dependenciesdir .. "/imguizmo"
includedir["spdlog"] = dependenciesdir .. "/spdlog/include/"
includedir["crossguid"] = dependenciesdir .. "/crossguid/include/"
includedir["hash"] = dependenciesdir .. "/Hash/include/"
includedir["jsonxx"] = dependenciesdir .. "/jsonxx/"
includedir["GLAD"] = "Wyrd/vendor/glad/include"

-- Local Libs
includedir["ImGuiEx"] = "libs/ImGuiEx/include"

-- if mono was found, the add the in application to the include directories
if monofound then
	includedir["mono"] = monodir .. "include/mono-2.0/"
end

-- if renderdoc was found, the add the in application to the include directories
if renderdocfound then
	includedir["renderdoc"] = renderdocdir
end

if os.istarget("windows") then
group "Third Party"
	include "buildsystem/common/premake/glfw.lua"
	include "buildsystem/common/premake/glad.lua"
	include "buildsystem/common/premake/glm.lua"
	include "buildsystem/common/premake/jsonxx.lua"
	include "buildsystem/common/premake/soil.lua"
	include "buildsystem/common/premake/imgui.lua"
	include "buildsystem/common/premake/imguizmo.lua"
	include "buildsystem/common/premake/spdlog.lua"
	include "buildsystem/common/premake/crossguid.lua"
end

if os.istarget("linux") then
group "Third Party"
	include "buildsystem/common/premake/glad.lua"
	include "buildsystem/common/premake/jsonxx.lua"
	include "buildsystem/common/premake/imgui.lua"
	include "buildsystem/common/premake/imguizmo.lua"
end

group ""
	project "Wyrd"
		location "Wyrd"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		characterset "Unicode"

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
			"dependencies/soil/src/",
			includedir["jsonxx"]
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
			"crossguid",
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
				"%{includedir.crossguid}"
			}
			
			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_LIBRARY_EXPORT",
				"GLFW_INCLUDE_NONE",
				"GLM_ENABLE_EXPERIMENTAL"
			}
			
			buildoptions
			{
				"/utf-8"
			}

		filter "system:linux"
			systemversion "latest"


   			includedirs
			{
				"%{prj.name}/src/"
			}

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

group "Tools"
if os.istarget("windows") then
	project "WyrdGen"
			location "WyrdGen"
			kind "ConsoleApp"
			language "C#"
			characterset "Unicode"

			targetdir ("lib/" .. outputdir .. "/%{prj.name}")
			objdir ("obj/" .. outputdir .. "/%{prj.name}")
			
			files
			{
				"%{prj.name}/**.cs",
				"%{prj.name}/**.xml",
				"%{prj.name}/**.template"
			}
			
			links { "System.Xml" }

			filter "system:windows"
				systemversion "latest"

				defines
				{
					"WYRD_PLATFORM_WINDOWS",
					"WYRD_EDITOR_ENABLED",
					"GLFW_INCLUDE_NONE",
					"GLM_ENABLE_EXPERIMENTAL"
				}
				
				buildoptions
				{
					"/utf-8"
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
end
group "Scripting"
	project "WyrdCAPI"
		location "WyrdCAPI"
		kind "SharedLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		characterset "Unicode"

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
			"%{includedir.jsonxx}",
			"%{includedir.crossguid}",
			"%{includedir.spdlog}"
		}
		
        links { 
			"uuid",
			"Wyrd",
			"crossguid"
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
			
			buildoptions
			{
				"/utf-8"
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
			
if os.istarget("windows") then
	project "WyrdAPI"
		location "WyrdAPI"
		kind "SharedLib"
		language "C#"
		characterset "Unicode"

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
			
			buildoptions
			{
				"/utf-8"
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
end

group "Libraries"
	project "ImGuiEx"
		location "libs/ImGuiEx"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		characterset "Unicode"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")

		files
		{
			"libs/%{prj.name}/**.cpp",
			"libs/%{prj.name}/**.h"
		}

		includedirs
		{
			"%{includedir.crossguid}",
			"%{includedir.glm}",
			"%{includedir.jsonxx}",
			"%{includedir.spdlog}",
			"%{includedir.imgui}",
			"%{includedir.ImGuiEx}",
			"Wyrd/src"
		}

		filter "system:windows"
			systemversion "latest"

			debugenvs { "PATH=%PATH%;%MONO_BIN%" }
			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"IMGUI_DEFINE_MATH_OPERATORS",
			}

			links
			{
				"GLFW",
				"GLAD",
				"SOIL",
				"jsonxx",
				"spdlog",
				"imgui",
				"opengl32.dll"
			}
			
			buildoptions
			{
				"/utf-8"
			}
			
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

group "Applications"
	project "TestPlayer"
		location "Apps/TestPlayer"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		characterset "Unicode"

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
			"%{includedir.crossguid}"
		}

		dependson 
		{ 
			"WyrdAPI", 
			"WyrdCAPI"
		}

		filter "system:windows"
			systemversion "latest"

			debugenvs { "PATH=%PATH%;%MONO_BIN%" }

			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_EDITOR_ENABLED",
				"GLM_ENABLE_EXPERIMENTAL",
				"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
				"MONO_INSTALL_LOC=" .. monodir
			}

			links
			{
				"Wyrd",
				"GLFW",
				"GLAD",
				"SOIL",
				"jsonxx",
				"imgui",
				"imguizmo",
				"opengl32.dll"
			}
				
			buildoptions
			{
				"/utf-8"
			}

			linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
		filter "system:linux"

			files
			{
				"Wyrd/vendor/glad/include/**.h",
				"Wyrd/vendor/glad/src/**.c",
			}

			libdirs
			{
				"buildsystem/linux/bin/Debug/glad/",
				"buildsystem/linux/bin/Debug/jsonxx/"
			}

			links
			{
				"Wyrd",
				"glfw3",
				"GLAD",
				"dl",
				"SOIL",
				"crossguid",
				"uuid",
				"spdlog",
				"mono-2.0",
				"pthread"
			}

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

	project "ImGuiTests"
		location "Apps/ImGuiTests"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		characterset "Unicode"
	
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
			"%{includedir.glm}",
			"%{includedir.GLFW}",
			"%{includedir.GLAD}",
			"%{includedir.jsonxx}",
			"%{includedir.SOIL}",
			"%{includedir.mono}",
			"%{includedir.spdlog}",
			"%{includedir.crossguid}",
			"%{includedir.imgui}",
			"%{includedir.ImGuiEx}",
		}
	
		dependson 
		{ 
			"WyrdAPI", 
			"WyrdCAPI"
		}
	
		filter "system:windows"
			systemversion "latest"
	
			debugenvs { "PATH=%PATH%;%MONO_BIN%" }
			
			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_EDITOR_ENABLED",
				"GLM_ENABLE_EXPERIMENTAL",
				"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
				"MONO_INSTALL_LOC=" .. monodir
			}
	
			links
			{
				"Wyrd",
				"GLFW",
				"GLAD",
				"SOIL",
				"jsonxx",
				"crossguid",
				"imgui",
				"ImGuiEx",
				"opengl32.dll"
			}
				
			buildoptions
			{
				"/utf-8"
			}
			
			linkoptions { "/WHOLEARCHIVE:Wyrd" }
		
		filter "system:linux"
	
			files
			{
				"Wyrd/vendor/glad/include/**.h",
				"Wyrd/vendor/glad/src/**.c",
			}
	
			libdirs
			{
				"buildsystem/linux/bin/Debug/glad/",
				"buildsystem/linux/bin/Debug/jsonxx/"
			}
	
			links
			{
				"Wyrd",
				"glfw3",
				"GLAD",
				"dl",
				"SOIL",
				"crossguid",
				"uuid",
				"spdlog",
				"mono-2.0",
				"pthread"
			}
	
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
		characterset "Unicode"

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
			"%{includedir.imguizmo}",
			"%{includedir.tinyobjloader}",
			"%{includedir.mono}",
			"%{includedir.spdlog}",
			"%{includedir.crossguid}",
			"%{includedir.hash}",
			iif(renderdocfound, includedir["renderdoc"], "")
		}

		dependson 
		{ 
			"WyrdAPI", 
			"WyrdCAPI",
			"TestPlayer"
		}

		filter "system:windows"
			systemversion "latest"

			debugenvs { "PATH=%PATH%;%MONO_BIN%" }
			defines
			{
				"WYRD_PLATFORM_WINDOWS",
				"WYRD_EDITOR_ENABLED",
				"GLM_ENABLE_EXPERIMENTAL",
				"IMGUI_DEFINE_MATH_OPERATORS",
				"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
				"MONO_INSTALL_LOC=" .. monodir,
				iif(renderdocfound, "WYRD_RENDERDOC_ENABLED", "")
			}

			links
			{
				"Wyrd",
				"GLFW",
				"GLAD",
				"SOIL",
				"jsonxx",
				"imgui",
				"imguizmo",
				"opengl32.dll"
			}
				
			buildoptions
			{
				"/utf-8"
			}

			linkoptions { "/WHOLEARCHIVE:Wyrd" }

		filter  "system:linux"
			defines
			{
				"WYRD_PLATFORM_LINUX",
				"WYRD_EDITOR_ENABLED",
				"GLM_ENABLE_EXPERIMENTAL",
				"NATIVE_API_LIB_LOC=" .. os.getcwd() .. "/lib/Debug/",
				"MONO_INSTALL_LOC=" .. monodir
			}

			libdirs
			{
				"buildsystem/linux/bin/Debug/glad/",
				"buildsystem/linux/bin/Debug/jsonxx/"
			}

			links
			{
				"Wyrd",
				"glfw3",
				"SOIL",
				"crossguid",
				"uuid",
				"glad",
				"imgui",
				"imguizmo",
				"jsonxx",
				"mono-2.0",
				"dl",
				"pthread"
			}

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
