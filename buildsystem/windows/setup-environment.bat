git clone https://github.com/glfw/glfw dependencies/glfw
cd dependencies/glfw
mkdir cmakedir
cd cmakedir
cmake ../
cd ../../..



git clone https://github.com/g-truc/glm dependencies/glm
cd dependencies/glm
mkdir cmakedir
cd cmakedir
cmake ../
cd ../../..

git clone https://github.com/hjiang/jsonxx dependencies/jsonxx

git clone -b docking https://github.com/ocornut/imgui.git dependencies/imgui

git clone https://github.com/CedricGuillemet/ImGuizmo.git dependencies/ImGuizmo

git clone https://github.com/gabime/spdlog dependencies/spdlog
cd dependencies/spdlog
mkdir cmakedir
cd cmakedir
cmake ../
cd ../../.. 

git clone https://github.com/graeme-hill/crossguid.git dependencies/crossguid
cd dependencies/crossguid
mkdir cmakedir
cd cmakedir
cmake ../
cd ../../..

git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library dependencies/soil
cd dependencies/soil
mkdir cmakedir
cd cmakedir
cmake ../
cd ../../.. 