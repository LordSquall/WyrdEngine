# Setup output folders
mkdir dependencies

###### Freetype ######
mkdir dependencies/freetype
git clone https://github.com/freetype/freetype.git dependencies/freetype
cd dependencies/freetype
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make -j 4
sudo make install
cd ../../..
