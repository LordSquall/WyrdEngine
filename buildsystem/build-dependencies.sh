# ensure all required header file and libs available
sudo apt install --assume-yes cmake xorg-dev libglu1-mesa-dev uuid-dev libglm-dev libglfw3 libglfw3-dev libsoil-dev

# Setup output folders
mkdir dependencies

###### Premake5 ######
mkdir dependencies/premake-core
git clone https://github.com/premake/premake-core.git dependencies/premake-core
cd dependencies/premake-core
make -f Bootstrap.mak linux

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

# Mono needs to be installed from a different repo
sudo apt install dirmngr gnupg apt-transport-https ca-certificates software-properties-common
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
sudo apt-add-repository 'deb https://download.mono-project.com/repo/ubuntu stable-focal main'
sudo apt install mono-complete 