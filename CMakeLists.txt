cmake_minimum_required (VERSION 3.9)

project (OsirisEngine VERSION 1.0.0 DESCRIPTION "Osiris Engine Shared Library")

# The Version Number
set (OsirisEngineLib_VERSION_MAJOR 1)
set (OsirisEngineLib_VERSION_MINOR 0)

# add source files to library 
set(OsirisEngineLib_SourceFiles
	engine/framework/framework.cpp
	engine/subsystems/window/MSWindow.cpp
	engine/subsystems/renderer/DX11Renderer.cpp
	engine/subsystems/renderer/OGLRenderer.cpp
)	

add_library(OsirisEngineLib SHARED
	${OsirisEngineLib_SourceFiles}
)

set(OSIRIS_lIBS OsirisEngineLib)

# add source files to application 
set(OsirisApp_SourceFiles
	application/ConsoleLogger.cpp
	application/main.cpp
)


include_directories(OsirisApp engine)	

add_executable(OsirisApp
	${OsirisApp_SourceFiles}
)

target_link_libraries(OsirisApp ${OSIRIS_lIBS})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OsirisApp)
