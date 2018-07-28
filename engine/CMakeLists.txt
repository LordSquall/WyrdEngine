cmake_minimum_required (VERSION 3.9)

# Add each of the subsystems 
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/framework")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/loaders")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/renderer")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/window")

include_directories(OsirisEngineLib "${PROJECT_SOURCE_DIR}/engine")	

add_library(OsirisEngineLib STATIC
	${ENGINE_SOURCE}
)

set(OSIRIS_LIBS 
OsirisEngineLib
PARENT_SCOPE
)