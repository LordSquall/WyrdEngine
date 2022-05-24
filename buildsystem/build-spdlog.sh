# Setup output folders
mkdir dependencies

##### SpdLog ######
mkdir dependencies/spdlog
git clone https://github.com/gabime/spdlog dependencies/spdlog
cd dependencies/spdlog
mkdir cmakedir
cd cmakedir
sudo cmake ../
sudo make
sudo make install
cd ../../.. 