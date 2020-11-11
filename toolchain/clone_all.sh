mkdir sub_modules
cp $PWD/toolchain/pull_all.sh $PWD/sub_modules
cd $PWD/sub_modules/

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

chmod 777 pull_all.sh
./pull_all.sh