cmake_minimum_required (VERSION 3.9)

list(APPEND CMAKE_PREFIX_PATH ${GLEW_ROOT})

MESSAGE(STATUS ${CMAKE_SOURCE_DIR} )
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/GLEW")
set(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/external/GLEW/lib/Release/Win32/")

find_package(OPENGL REQUIRED)
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

set(OSIRIS_LIBS
${OSIRIS_LIBS}
${GLEW_LIBRARIES}
PARENT_SCOPE
)

set(OSIRIS_INCLUDE_DIRS
${OSIRIS_INCLUDE_DIRS}
${GLEW_INCLUDE_DIRS}
PARENT_SCOPE
)

# Sub-System source files to include within the build
set(SRC_SS_RENDERER
   ${CMAKE_CURRENT_SOURCE_DIR}/DX11Renderer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/OGLRenderer.cpp
)

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_SS_RENDERER}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_SS_RENDERER_GROUP
   ${SRC_SS_RENDERER}
   PARENT_SCOPE
)