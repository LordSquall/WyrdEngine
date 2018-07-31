cmake_minimum_required (VERSION 3.9)

# Sub-System source files to include within the build
set(SRC_FRAMEWORK
   ${CMAKE_CURRENT_SOURCE_DIR}/Framework.cpp
)

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_FRAMEWORK}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_FRAMEWORK_GROUP
   ${SRC_FRAMEWORK}
   PARENT_SCOPE
)
