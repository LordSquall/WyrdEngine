cmake_minimum_required (VERSION 3.9)

# Sub-System source files to include within the build
set(SRC_SS_GRAPHICS
   ${CMAKE_CURRENT_SOURCE_DIR}/BatchTable.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Texture.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/VertexArray.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.cpp
)

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_SS_GRAPHICS}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_SS_GRAPHICS_GROUP
   ${SRC_SS_GRAPHICS}
   PARENT_SCOPE
)
