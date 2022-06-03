echo off

mkdir dependencies

::###### GLFW ######
git clone https://github.com/glfw/glfw dependencies/glfw

::###### SOIL ######
git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library dependencies/soil

::###### GLM ######
git clone https://github.com/g-truc/glm dependencies/glm

::###### Mono ######
git clone https://github.com/mono/mono.git dependencies/mono

::###### Crossguid ######
git clone https://github.com/graeme-hill/crossguid.git dependencies/uuid

::###### SpdLog ######
git clone https://github.com/gabime/spdlog dependencies/spdlog

::###### ImGui ######
git clone -b Docking https://github.com/ocornut/imgui.git dependencies/imgui

::###### jsonxx ######
git clone https://github.com/hjiang/jsonxx.git dependencies/jsonxx

::###### FreeType ######
git clone https://github.com/freetype/freetype.git dependencies/freetype