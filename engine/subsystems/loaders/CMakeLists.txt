cmake_minimum_required (VERSION 3.9)

# Sub-System source files to include within the build
set(SRC_SS_LOADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/ConfigLoader.cpp
)

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
