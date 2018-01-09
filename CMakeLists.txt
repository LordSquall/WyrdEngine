cmake_minimum_required (VERSION 3.9)

project (OrisisEngine VERSION 1.0.0 DESCRIPTION "Orisis Engine Shared Library")

# The Version Number
set (OsirisEngineLib_VERSION_MAJOR 1)
set (OrisisEngineLib_VERSION_MINOR 0)

# add source files to library 
set(OsirisEngineLib_SourceFiles
	engine/framework/framework.cpp
)	

add_library(OrisisEngineLib SHARED
	${OsirisEngineLib_SourceFiles}
)

# add source files to application 
set(OsirisApp_SourceFiles
	application/main.cpp
)	

add_executable(OrisisApp
	${OsirisApp_SourceFiles}
)

#target_link_libraries(OrisisApp OrisisEngineLib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OrisisApp)
