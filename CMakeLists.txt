cmake_minimum_required (VERSION 3.9)

project (OsirisEngine VERSION 1.0.0 DESCRIPTION "Osiris Engine Shared Library")

# The Version Number
set (OsirisEngineLib_VERSION_MAJOR 1)
set (OsirisEngineLib_VERSION_MINOR 0)

# Add the engine library into the project
add_subdirectory("${PROJECT_SOURCE_DIR}/engine")

# Add source files to application 
set(OsirisApp_SourceFiles
	application/ConsoleLogger.cpp
	application/main.cpp
)

# Set the includes for the engine header files
include_directories(OsirisApp engine)	

# Add a executable using the sample application code
add_executable(OsirisApp WIN32
	${OsirisApp_SourceFiles}
)

# Link the osiris engine 
MESSAGE(STATUS ${OSIRIS_LIBS})
target_link_libraries(OsirisApp ${OSIRIS_LIBS})

# Set the default project to be the sample app
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OsirisApp)
