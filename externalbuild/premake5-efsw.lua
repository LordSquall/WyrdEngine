externallibsdir = os.getenv("OSIRIS_EXTERN_REPOS")

project "efsw"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
    
	src_dir = externallibsdir .. "/efsw/src/"
	include_dir = externallibsdir .. "/efsw/include/"
	
	basedir("../")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        src_dir .. "efsw/*.cpp",
        src_dir .. "efsw/*.hpp"
    }
	
	includedirs
	{
		include_dir,
		src_dir				--Source added for include due to hpp fies
    }
    
	filter "system:windows"
		systemversion "latest"
		
		files
        {
			src_dir .. "efsw/platform/win/*.cpp"
        }
		
		excludes { 
			src_dir .. "efsw/WatcherKqueue.cpp", 
			src_dir .. "efsw/WatcherFSEvents.cpp", 
			src_dir .. "efsw/WatcherInotify.cpp", 
			src_dir .. "efsw/FileWatcherKqueue.cpp", 
			src_dir .. "efsw/FileWatcherInotify.cpp", 
			src_dir .. "efsw/FileWatcherFSEvents.cpp" 
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Debug"
		symbols "on"