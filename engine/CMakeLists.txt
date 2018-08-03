cmake_minimum_required (VERSION 3.9)

# Add each of the subsystems 
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/framework")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/loaders")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/renderer")
add_subdirectory("${PROJECT_SOURCE_DIR}/engine/subsystems/window")

include_directories(OsirisEngineLib "${PROJECT_SOURCE_DIR}/engine" ${OSIRIS_INCLUDE_DIRS})	

add_library(OsirisEngineLib STATIC ${SRC_ENGINE})

# Create the source folder group
source_group("framework" FILES ${SRC_FRAMEWORK_GROUP})
source_group("subsystems\\loaders" FILES ${SRC_SS_LOADERS_GROUP})
source_group("subsystems\\renderer" FILES ${SRC_SS_RENDERER_GROUP})
source_group("subsystems\\window" FILES ${SRC_SS_WINDOW_GROUP})

set(OSIRIS_INCLUDE_DIRS
${OSIRIS_INCLUDE_DIRS}
PARENT_SCOPE
)

set(OSIRIS_LIBS
${OSIRIS_LIBS} 
OsirisEngineLib
PARENT_SCOPE
)