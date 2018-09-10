cmake_minimum_required (VERSION 3.9)

# Sub-System source files to include within the build
set(SRC_SS_SPRITES
   ${CMAKE_CURRENT_SOURCE_DIR}/Sprite.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SpriteLayer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SpriteSystem.cpp
)

# DO NOT EDIT BELOW THIS LINE
# Add sub-systems files to the source variable
set(SRC_ENGINE
   ${SRC_SS_SPRITES}
   ${SRC_ENGINE}
   PARENT_SCOPE
)

set(SRC_SS_SPRITES_GROUP
   ${SRC_SS_SPRITES}
   PARENT_SCOPE
)
