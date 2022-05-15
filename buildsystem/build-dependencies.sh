# Setup output folders
mkdir dependencies/bin
mkdir dependencies/lib

###### Cross guid ######
mkdir dependencies/uuid
git clone https://github.com/graeme-hill/crossguid.git dependencies/uuid

cd dependencies/uuid
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
cd ../../..
ln -s dependencies/uuid/cmakedir/libcrossguid.a  dependencies/lib/libcrossguid.a 
