# Setup output folders
mkdir dependencies

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
