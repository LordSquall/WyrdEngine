cmake_minimum_required (VERSION 3.9)

# External source files to include within the build
set(SRC_EXT_BMPREAD
   ${CMAKE_CURRENT_SOURCE_DIR}/bmpread.cpp
)

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_EXT_BMPREAD}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_EXT_BMPREAD_GROUP
   ${SRC_EXT_BMPREAD}
   PARENT_SCOPE
)
