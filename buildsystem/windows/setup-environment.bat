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

:: download and install mono for windows
curl --output mono-install.msi --url https://download.mono-project.com/archive/6.12.0/windows-installer/mono-6.12.0.182-x64-0.msi
mono-install.msi /passive

export PREFIX=[mono installation location]
export PATH=$PREFIX/bin:$PATH
git clone https://github.com/mono/mono.git dependencies/mono
cd dependencies/mono
mkdir cmakedir
cd cmakedir
cd ../../.. 