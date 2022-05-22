# ensure all required header file and libs available
sudo apt install cmake
sudo apt install xorg-dev 
sudo apt install libglu1-mesa-dev
sudo apt install uuid-dev
sudo apt install libglm-dev

# Mono needs to be installed from a different repo
sudo apt install dirmngr gnupg apt-transport-https ca-certificates software-properties-common
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
sudo apt-add-repository 'deb https://download.mono-project.com/repo/ubuntu stable-focal main'
sudo apt install mono-complete 
sudo ln -s /usr/include/mono-2.0/mono /usr/include/mono

# create toolchain dir to store any build tools
mkdir toolchain 

# create directory and clone premake
# premake5 is not available to install as a binary so we need to build it
git clone https://github.com/premake/premake-core.git ./toolchain/premake-core
cd toolchain/premake-core
make -f Bootstrap.mak linux
