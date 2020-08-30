cd ..
mkdir sub_modules
MOVE /Y %CD%\toolchain\pull_all.bat %CD%\sub_modules
cd %CD%\sub_modules\

git clone https://github.com/glfw/glfw
git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library
git clone https://github.com/g-truc/glm
git clone --branch docking https://github.com/ocornut/imgui
git clone https://github.com/lua/lua
git clone https://github.com/vinniefalco/LuaBridge
git clone https://github.com/gabime/spdlog
git clone https://github.com/SpartanJ/efsw
git clone https://github.com/hjiang/jsonxx.git
git clone https://github.com/mono/mono.git

setx OSIRIS_EXTERN_REPOS %CD%
call pull_all.bat
pause