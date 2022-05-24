# Setup output folders
mkdir dependencies
mkdir dependencies/bin
mkdir dependencies/lib

###### GLM ######
mkdir dependencies/glfw
git clone https://github.com/glfw/glfw.git dependencies/glfw
cd dependencies/glfw
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
cd ../../..
ln -s dependencies/uuid/cmakedir/src/libglfw3.a  dependencies/lib/libglfw3.a 

###### SOIL ######
mkdir dependencies/soil
git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library dependencies/soil
cd dependencies/soil
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
cd ../../..
ln -s dependencies/soil/cmakedir/libSOIL.a  dependencies/lib/libSOIL.a 

###### Crossguid ######
mkdir dependencies/uuid
git clone https://github.com/graeme-hill/crossguid.git dependencies/uuid
cd dependencies/uuid
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
sudo make install
cd ../../..

###### jsonxx ######
mkdir dependencies/jsonxx
git clone https://github.com/hjiang/jsonxx.git dependencies/jsonxx
cd dependencies/jsonxx
sudo make
sudo make install
cd ../..

###### SOIL ######
mkdir dependencies/spdlog
git clone https://github.com/gabime/spdlog dependencies/spdlog
cd dependencies/spdlog
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
sudo make install
cd ../../..