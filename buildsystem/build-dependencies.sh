# Setup output folders
mkdir dependencies
mkdir dependencies/bin
mkdir dependencies/lib

###### GLM ######
mkdir dependencies/glm
git clone https://github.com/g-truc/glm.git dependencies/glm
cd dependencies/glm
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make -j 4
sudo make install
cd ../../..

###### SOIL ######
mkdir dependencies/soil
git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library dependencies/soil
cd dependencies/soil
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make -j 4
sudo make install
cd ../../..

###### Crossguid ######
mkdir dependencies/uuid
git clone https://github.com/graeme-hill/crossguid.git dependencies/uuid
cd dependencies/uuid
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make -j 4
sudo make install
cd ../../..

###### jsonxx ######
mkdir dependencies/jsonxx
git clone https://github.com/hjiang/jsonxx.git dependencies/jsonxx
cd dependencies/jsonxx
sudo make -j 4
sudo make install
cd ../..

###### SOIL ######
mkdir dependencies/spdlog
git clone https://github.com/gabime/spdlog dependencies/spdlog
cd dependencies/spdlog
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make -j 4
sudo make install
cd ../../..