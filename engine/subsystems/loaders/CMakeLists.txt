cmake_minimum_required (VERSION 3.9)

# Sub-System source files to include within the build
set(SRC_SS_LOADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/BitmapLoader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ConfigLoader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ShaderLoader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SpriteLayerLoader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SpriteLoader.cpp
)

# JSON
set(OSIRIS_INCLUDE_DIRS
${OSIRIS_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/external/json/include
PARENT_SCOPE
)

# BMPREAD
add_subdirectory("${PROJECT_SOURCE_DIR}/external/bmpread" "${PROJECT_SOURCE_DIR}/external/bmpread")

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_SS_LOADERS}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_SS_LOADERS_GROUP
   ${SRC_SS_LOADERS}
   PARENT_SCOPE
)
